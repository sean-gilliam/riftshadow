#ifndef STDLIBS_HANDLE_H
#define STDLIBS_HANDLE_H

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

//
// A generational handle and the slot map that issues it.
//
// The problem this solves: the recycled entity types hand out reused
// addresses. When one is freed it goes onto a free list and the very next
// allocation can return the same address, so a raw back-reference to a dead
// entity does not merely dangle. It can silently start pointing at whatever
// took its place. Today that is policed by a `valid` bool plus hand-written
// scrub loops that walk every entity nulling inbound references to the one
// being freed.
//
// A handle is that `valid` check folded into the reference itself. It names a
// slot and the generation of that slot at the moment the handle was made.
// Deref compares generations, so a handle to a freed entity resolves to null
// forever. No scrub loop required, and no chance of resolving to the
// entity's replacement.
//
// Two invariants carry most of the weight:
//
//   * Generation 0 is reserved and never issued, so an all-zero handle is a
//     null handle. Entities are reset by value-initializing over them and a
//     few places still clear memory wholesale; both must produce a null
//     reference rather than one pointing at slot 0.
//
//   * A slot whose generation counter is spent is retired permanently rather
//     than wrapped. Wrapping is the one way a generational handle can still
//     resolve to the wrong object, and it is silent. No sanitizer sees it.
//     Burning a slot is far cheaper than that.
//
// Note there is deliberately no `operator bool` and no `operator->`. A handle
// being non-null does not mean it is live, so a truthiness test would read
// like the raw-pointer check it replaces while answering a different question.
// Every read goes through Deref, which forces the liveness check to be
// visible at the point of use.
//

template <typename T, typename GenT>
class SlotMap;

/// A non-owning, generation-checked reference to an entity held in a SlotMap.
/// Trivially copyable and 64 bits wide, so it can be stored in the entity
/// structs anywhere a raw pointer is stored today.
template <typename T, typename GenT = std::uint32_t>
class Handle
{
public:
	Handle() = default;

	/// Lets an existing `x = nullptr` clear a field that used to be a pointer.
	Handle(std::nullptr_t) {}

	/// True when this handle names nothing at all. Says nothing about whether
	/// the entity it names is still alive. Only Deref answers that.
	bool IsNull() const { return generation == 0; }

	bool operator==(const Handle &other) const
	{
		return index == other.index && generation == other.generation;
	}

	bool operator!=(const Handle &other) const { return !(*this == other); }

	// Comparing against nullptr is deliberately a compile error, even though
	// assigning nullptr is not. Without these the converting constructor above
	// makes `h == nullptr` compile and quietly mean "was never assigned",
	// which is not the question a raw-pointer null check used to ask -- that
	// one meant "is there something there", and the handle's answer to it is
	// Deref(h) == nullptr. The two differ for exactly the case handles exist
	// to catch: a reference to an entity that has since been freed.
	//
	// Deleting them puts those sites back in front of a human, which is the
	// same reason there is no operator bool and no operator->.
	bool operator==(std::nullptr_t) const = delete;
	bool operator!=(std::nullptr_t) const = delete;

	/// Hashes the handle's identity, so a handle can key an unordered_map.
	///
	/// This is the one thing worth having that a raw pointer key does not do:
	/// a handle to a freed entity never compares equal to a handle to whatever
	/// entity later takes its address, because the generation differs. A map
	/// keyed on pointers silently answers the old entity's questions with the
	/// new entity's entry.
	///
	/// Says nothing about liveness. An expired handle keeps its hash, which is
	/// what makes stale entries findable and droppable rather than confusable.
	/// Exposing this does not weaken the deliberate omissions above: it yields a
	/// number, not a way to reach the entity, so every read still goes through
	/// Deref.
	std::size_t Hash() const
	{
		return (static_cast<std::size_t>(index) << 32) ^ static_cast<std::size_t>(generation);
	}

private:
	friend class SlotMap<T, GenT>;

	std::uint32_t	index = 0;
	GenT			generation = 0;		// 0 is reserved: an all-zero handle is null.
};

/// Issues handles for entities and answers whether a handle is still good.
///
/// The map does not own the entities and never moves them. It stores plain
/// pointers to objects allocated elsewhere. That is not an oversight: the
/// codebase caches raw entity pointers across list mutations all over the
/// place, so storage that relocates its elements would break those callers.
/// Registering a pointer here changes nothing about who allocates or frees it.
///
/// GenT is the width of the generation counter. It exists so tests can
/// exhaust a counter in reasonable time; production callers want the default.
template <typename T, typename GenT = std::uint32_t>
class SlotMap
{
public:
	using HandleType = Handle<T, GenT>;

	/// Registers an entity and returns a handle to it. A null entity gets a
	/// null handle and consumes no slot.
	HandleType Add(T *entity)
	{
		HandleType handle;

		if (entity == nullptr)
			return handle;

		std::uint32_t slotIndex;

		if (freeHead == NO_SLOT)
		{
			slotIndex = static_cast<std::uint32_t>(slots.size());
			slots.push_back(Slot());
		}
		else
		{
			slotIndex = freeHead;
			freeHead = slots[slotIndex].nextFree;
		}

		Slot &slot = slots[slotIndex];
		slot.entity = entity;
		slot.nextFree = NO_SLOT;
		liveCount++;

		handle.index = slotIndex;
		handle.generation = slot.generation;

		return handle;
	}

	/// Expires every handle to this entity. Removing something already removed
	/// is a silent no-op, matching how the free_X functions behave today.
	/// It matters because bumping the generation on a stray call would
	/// spend the slot's counter for nothing.
	void Remove(HandleType handle)
	{
		if (Find(handle) == nullptr)
			return;

		Slot &slot = slots[handle.index];
		slot.entity = nullptr;
		liveCount--;

		// A spent counter retires the slot. It is never reused, so the handles
		// already issued against it stay expired for good.
		if (slot.generation == std::numeric_limits<GenT>::max())
			return;

		slot.generation++;
		slot.nextFree = freeHead;
		freeHead = handle.index;
	}

	/// The entity this handle names, or null if the handle is null, expired,
	/// or malformed. This is the only way to read through a handle.
	T *Deref(HandleType handle) const
	{
		const Slot *slot = Find(handle);

		return slot != nullptr ? slot->entity : nullptr;
	}

	bool IsLive(HandleType handle) const { return Deref(handle) != nullptr; }

	/// Entities currently registered.
	std::size_t LiveCount() const { return liveCount; }

	/// Slots ever allocated, live plus free plus retired. Only interesting for
	/// tests and for watching slot churn over a long run.
	std::size_t SlotCount() const { return slots.size(); }

private:
	static constexpr std::uint32_t NO_SLOT = ~static_cast<std::uint32_t>(0);

	struct Slot
	{
		T *				entity = nullptr;	// null while the slot is free or retired
		GenT			generation = 1;		// starts at 1; 0 is the reserved null generation
		std::uint32_t	nextFree = NO_SLOT;
	};

	const Slot *Find(HandleType handle) const
	{
		if (handle.IsNull() || handle.index >= slots.size())
			return nullptr;

		const Slot &slot = slots[handle.index];

		if (slot.generation != handle.generation || slot.entity == nullptr)
			return nullptr;

		return &slot;
	}

	std::vector<Slot>	slots;
	std::uint32_t		freeHead = NO_SLOT;
	std::size_t			liveCount = 0;
};

/// Lets a handle be an unordered_map key. See Handle::Hash for why that is
/// worth having over a raw pointer key.
namespace std
{
	template <typename T, typename GenT>
	struct hash<Handle<T, GenT>>
	{
		std::size_t operator()(const Handle<T, GenT> &handle) const { return handle.Hash(); }
	};
}

#endif /* STDLIBS_HANDLE_H */
