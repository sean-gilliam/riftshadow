<?php

use Illuminate\Database\Seeder;

class SeedWorldAreasTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		DB::database('rift_core')
			->table('world_areas')
			->insert([
				[
					'id' => 0,
					'name' => 'Limbo',
					'credits' => 'Rift',
					'low_range' => 52,
					'high_range' => 60,
					'min_vnum' => 1,
					'max_vnum' => 99,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 1,
					'name' => 'Test Shrine',
					'credits' => 'None',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 100,
					'max_vnum' => 110,
					'type' => 6,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 2,
					'name' => 'Eluviel\'s Shrine',
					'credits' => 'Eluviel',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 111,
					'max_vnum' => 119,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 3,
					'name' => 'Zethus\'s Palace',
					'credits' => 'Zethus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 120,
					'max_vnum' => 129,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 4,
					'name' => 'The Shrine of Morglum',
					'credits' => 'Morglum',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 130,
					'max_vnum' => 154,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 5,
					'name' => 'The Shrine of Waekath',
					'credits' => 'Waekath',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 155,
					'max_vnum' => 179,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 6,
					'name' => 'The Shrine of Tiernus',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 180,
					'max_vnum' => 184,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 7,
					'name' => 'The Shrine of Seikilos',
					'credits' => 'Seikilos',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 185,
					'max_vnum' => 199,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 8,
					'name' => 'Mount Cim',
					'credits' => 'Zethus',
					'low_range' => 20,
					'high_range' => 51,
					'min_vnum' => 200,
					'max_vnum' => 299,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 9,
					'name' => 'Plains',
					'credits' => 'Copper',
					'low_range' => 1,
					'high_range' => 20,
					'min_vnum' => 300,
					'max_vnum' => 399,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 10,
					'name' => 'The Antava Marsh',
					'credits' => 'Sekh',
					'low_range' => 15,
					'high_range' => 30,
					'min_vnum' => 400,
					'max_vnum' => 499,
					'type' => 0,
					'climate' => 6,
					'flags' => 0,
				],
				[
					'id' => 13,
					'name' => 'Old Southern Road',
					'credits' => 'Karsus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 500,
					'max_vnum' => 599,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 13,
					'name' => 'Old Southern Road',
					'credits' => 'Karsus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 600,
					'max_vnum' => 699,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 13,
					'name' => 'Old Southern Road',
					'credits' => 'Karsus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 700,
					'max_vnum' => 799,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 14,
					'name' => 'Valley of Gehir',
					'credits' => 'Jathruk',
					'low_range' => 10,
					'high_range' => 51,
					'min_vnum' => 800,
					'max_vnum' => 899,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 15,
					'name' => 'Melcene',
					'credits' => 'Eluviel/Zethus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 900,
					'max_vnum' => 1199,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 16,
					'name' => 'Caelumaedan',
					'credits' => 'ROM',
					'low_range' => 51,
					'high_range' => 60,
					'min_vnum' => 1200,
					'max_vnum' => 1299,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 17,
					'name' => 'High Tower',
					'credits' => 'Skylar',
					'low_range' => 10,
					'high_range' => 30,
					'min_vnum' => 1300,
					'max_vnum' => 1499,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 18,
					'name' => 'Estates of Adrianna',
					'credits' => 'Karsus',
					'low_range' => 10,
					'high_range' => 30,
					'min_vnum' => 1500,
					'max_vnum' => 1599,
					'type' => 5,
					'climate' => 8,
					'flags' => 0,
				],
				[
					'id' => 19,
					'name' => 'Tahluir\'rethan',
					'credits' => 'Eluviel',
					'low_range' => 45,
					'high_range' => 51,
					'min_vnum' => 1600,
					'max_vnum' => 1699,
					'type' => 0,
					'climate' => 0,
					'flags' => 1,
				],
				[
					'id' => 20,
					'name' => 'Forest of Sarloth',
					'credits' => 'Eluviel',
					'low_range' => 1,
					'high_range' => 20,
					'min_vnum' => 1700,
					'max_vnum' => 1899,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 21,
					'name' => 'Temple of Cetan\'Det',
					'credits' => 'Kolaius',
					'low_range' => 1,
					'high_range' => 15,
					'min_vnum' => 1900,
					'max_vnum' => 1999,
					'type' => 5,
					'climate' => 6,
					'flags' => 0,
				],
				[
					'id' => 22,
					'name' => 'Catacombs',
					'credits' => 'Raff',
					'low_range' => 10,
					'high_range' => 20,
					'min_vnum' => 2000,
					'max_vnum' => 2099,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 23,
					'name' => 'Dawnview Monastery',
					'credits' => 'Karsus',
					'low_range' => 25,
					'high_range' => 35,
					'min_vnum' => 2100,
					'max_vnum' => 2199,
					'type' => 5,
					'climate' => 6,
					'flags' => 0,
				],
				[
					'id' => 24,
					'name' => 'Sewers of Cimar',
					'credits' => 'Karsus',
					'low_range' => 20,
					'high_range' => 35,
					'min_vnum' => 2200,
					'max_vnum' => 2399,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 25,
					'name' => 'Cimar',
					'credits' => 'Seikilos',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 2400,
					'max_vnum' => 2799,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 26,
					'name' => 'Nightwalker Caves',
					'credits' => 'Karsus',
					'low_range' => 20,
					'high_range' => 35,
					'min_vnum' => 2800,
					'max_vnum' => 2899,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 27,
					'name' => 'The Skills File',
					'credits' => 'Riftshadow',
					'low_range' => 1,
					'high_range' => 60,
					'min_vnum' => 2900,
					'max_vnum' => 2999,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 28,
					'name' => 'Rodit\'s Area',
					'credits' => 'Rodit',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 3000,
					'max_vnum' => 3099,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 29,
					'name' => 'Chasm Tower',
					'credits' => 'Gatog',
					'low_range' => 20,
					'high_range' => 51,
					'min_vnum' => 3100,
					'max_vnum' => 3199,
					'type' => 5,
					'climate' => 8,
					'flags' => 0,
				],
				[
					'id' => 30,
					'name' => 'Cim Caverns',
					'credits' => 'Waekath',
					'low_range' => 10,
					'high_range' => 20,
					'min_vnum' => 3200,
					'max_vnum' => 3299,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 31,
					'name' => 'The Ur\'Tazi Caves',
					'credits' => 'Rudem',
					'low_range' => 30,
					'high_range' => 40,
					'min_vnum' => 3300,
					'max_vnum' => 3399,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 32,
					'name' => 'The Tombs of Carantoc',
					'credits' => 'Zethus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 3400,
					'max_vnum' => 3499,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 33,
					'name' => 'The Great Shalaran Road',
					'credits' => 'Riftshadow',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 3500,
					'max_vnum' => 3699,
					'type' => 1,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 34,
					'name' => 'The Jungles of Tieran Loch',
					'credits' => 'Metaphiusen',
					'low_range' => 20,
					'high_range' => 40,
					'min_vnum' => 3700,
					'max_vnum' => 3799,
					'type' => 5,
					'climate' => 3,
					'flags' => 0,
				],
				[
					'id' => 35,
					'name' => 'Iseldheim',
					'credits' => 'Zethus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 3800,
					'max_vnum' => 3899,
					'type' => 4,
					'climate' => 4,
					'flags' => 0,
				],
				[
					'id' => 36,
					'name' => 'The Forgotten Tower',
					'credits' => 'Metaphiusen',
					'low_range' => 40,
					'high_range' => 51,
					'min_vnum' => 3900,
					'max_vnum' => 3999,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 37,
					'name' => 'Ruins of Malkeran',
					'credits' => 'Karsus',
					'low_range' => 0,
					'high_range' => 0,
					'min_vnum' => 4000,
					'max_vnum' => 4099,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 38,
					'name' => 'Sidhe Village',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 4100,
					'max_vnum' => 4249,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 39,
					'name' => 'Forgotten Hills',
					'credits' => 'Rhuann',
					'low_range' => 10,
					'high_range' => 0,
					'min_vnum' => 4300,
					'max_vnum' => 4399,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 40,
					'name' => 'The Fallen Keep',
					'credits' => 'Rendsin',
					'low_range' => 20,
					'high_range' => 30,
					'min_vnum' => 4400,
					'max_vnum' => 4599,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 41,
					'name' => 'The Elven Stronghold',
					'credits' => 'Wispen',
					'low_range' => 20,
					'high_range' => 51,
					'min_vnum' => 4600,
					'max_vnum' => 4799,
					'type' => 0,
					'climate' => 0,
					'flags' => 14,
				],
				[
					'id' => 42,
					'name' => 'The Tribal Grounds',
					'credits' => 'Tymari',
					'low_range' => 10,
					'high_range' => 25,
					'min_vnum' => 4800,
					'max_vnum' => 4899,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 43,
					'name' => 'Thalos',
					'credits' => 'Drkside',
					'low_range' => 10,
					'high_range' => 25,
					'min_vnum' => 5200,
					'max_vnum' => 5299,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 44,
					'name' => 'Haon Dor',
					'credits' => 'Diku',
					'low_range' => 5,
					'high_range' => 10,
					'min_vnum' => 6000,
					'max_vnum' => 6199,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 45,
					'name' => 'Mausoleum',
					'credits' => 'Khargqw',
					'low_range' => 25,
					'high_range' => 51,
					'min_vnum' => 6200,
					'max_vnum' => 6299,
					'type' => 0,
					'climate' => 0,
					'flags' => 1,
				],
				[
					'id' => 46,
					'name' => 'Sewers',
					'credits' => 'Diku',
					'low_range' => 5,
					'high_range' => 30,
					'min_vnum' => 7000,
					'max_vnum' => 7499,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 47,
					'name' => 'Floating Continent',
					'credits' => 'Zethus',
					'low_range' => 40,
					'high_range' => 51,
					'min_vnum' => 7500,
					'max_vnum' => 7599,
					'type' => 5,
					'climate' => 6,
					'flags' => 0,
				],
				[
					'id' => 48,
					'name' => 'The Sky Palace',
					'credits' => 'zethus',
					'low_range' => 47,
					'high_range' => 51,
					'min_vnum' => 7600,
					'max_vnum' => 7699,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 49,
					'name' => 'Valley of the Elves',
					'credits' => 'Hatchet',
					'low_range' => 5,
					'high_range' => 20,
					'min_vnum' => 7800,
					'max_vnum' => 7899,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 50,
					'name' => 'Valley of Ilopheth',
					'credits' => 'Tiernus',
					'low_range' => 8,
					'high_range' => 40,
					'min_vnum' => 9000,
					'max_vnum' => 9199,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 51,
					'name' => 'Elemental Canyon',
					'credits' => 'Raff',
					'low_range' => 5,
					'high_range' => 30,
					'min_vnum' => 9200,
					'max_vnum' => 9299,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 52,
					'name' => 'Sanihar il Dur',
					'credits' => 'Vornak',
					'low_range' => 30,
					'high_range' => 45,
					'min_vnum' => 9300,
					'max_vnum' => 9499,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 53,
					'name' => 'Gannyte Shoals',
					'credits' => 'Trevelyan',
					'low_range' => 1,
					'high_range' => 30,
					'min_vnum' => 10000,
					'max_vnum' => 10199,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 54,
					'name' => 'The Dragon Tower',
					'credits' => 'Barsak',
					'low_range' => 35,
					'high_range' => 51,
					'min_vnum' => 11400,
					'max_vnum' => 11599,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 55,
					'name' => 'Udgaard',
					'credits' => 'Pet/Aeo',
					'low_range' => 20,
					'high_range' => 40,
					'min_vnum' => 11600,
					'max_vnum' => 11799,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 56,
					'name' => 'Koteras Village',
					'credits' => 'Metaphiusen',
					'low_range' => 1,
					'high_range' => 30,
					'min_vnum' => 11800,
					'max_vnum' => 11899,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 57,
					'name' => 'The Valley Cantrevs',
					'credits' => 'Metaphiusen',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 11900,
					'max_vnum' => 12099,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 58,
					'name' => 'Duergar Mines',
					'credits' => 'Berand',
					'low_range' => 45,
					'high_range' => 51,
					'min_vnum' => 12200,
					'max_vnum' => 12299,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 59,
					'name' => 'Rivenrock Canyon',
					'credits' => 'Kyriss',
					'low_range' => 5,
					'high_range' => 35,
					'min_vnum' => 12300,
					'max_vnum' => 12399,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 60,
					'name' => 'The Dale',
					'credits' => 'Ceran',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 13500,
					'max_vnum' => 13699,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 61,
					'name' => 'Ruins of Drannor',
					'credits' => 'Ceran',
					'low_range' => 30,
					'high_range' => 51,
					'min_vnum' => 13700,
					'max_vnum' => 13899,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 62,
					'name' => 'Rakshasa Tower',
					'credits' => 'Ceran',
					'low_range' => 30,
					'high_range' => 51,
					'min_vnum' => 13900,
					'max_vnum' => 13999,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 63,
					'name' => 'Beholder Corps',
					'credits' => 'Ceran',
					'low_range' => 30,
					'high_range' => 51,
					'min_vnum' => 14000,
					'max_vnum' => 14050,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 64,
					'name' => 'The Crimson Sea',
					'credits' => 'Sheil',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 14200,
					'max_vnum' => 14299,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 65,
					'name' => 'The Naiadian Sea',
					'credits' => 'Kyriss',
					'low_range' => 20,
					'high_range' => 40,
					'min_vnum' => 15800,
					'max_vnum' => 15899,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 66,
					'name' => 'The Glimmering Forest',
					'credits' => 'Karsus',
					'low_range' => 0,
					'high_range' => 0,
					'min_vnum' => 15900,
					'max_vnum' => 15999,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 67,
					'name' => 'The Obsidian Order',
					'credits' => 'Barsak',
					'low_range' => 25,
					'high_range' => 51,
					'min_vnum' => 18000,
					'max_vnum' => 18099,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 68,
					'name' => 'Gehir Keep',
					'credits' => 'Jathruk',
					'low_range' => 0,
					'high_range' => 0,
					'min_vnum' => 18100,
					'max_vnum' => 18199,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 69,
					'name' => 'The Barren Hollows',
					'credits' => 'Zethus',
					'low_range' => 0,
					'high_range' => 0,
					'min_vnum' => 18200,
					'max_vnum' => 18299,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 70,
					'name' => 'Emerald Forest',
					'credits' => 'FeaR',
					'low_range' => 15,
					'high_range' => 51,
					'min_vnum' => 19201,
					'max_vnum' => 19299,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 71,
					'name' => 'Ruins of the Deep',
					'credits' => 'Teiphicker',
					'low_range' => 45,
					'high_range' => 51,
					'min_vnum' => 20100,
					'max_vnum' => 20299,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 72,
					'name' => 'Naiadia',
					'credits' => 'Kyriss',
					'low_range' => 1,
					'high_range' => 25,
					'min_vnum' => 20400,
					'max_vnum' => 20599,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 73,
					'name' => 'Dagdan',
					'credits' => 'Macheath',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 21500,
					'max_vnum' => 21599,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 74,
					'name' => 'Aran\'gird',
					'credits' => 'Berand',
					'low_range' => 45,
					'high_range' => 51,
					'min_vnum' => 21800,
					'max_vnum' => 21999,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 75,
					'name' => 'Varggin Shih',
					'credits' => 'Barsak',
					'low_range' => 40,
					'high_range' => 51,
					'min_vnum' => 22000,
					'max_vnum' => 22099,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 76,
					'name' => 'Forest of Myth Drannor',
					'credits' => 'Ceran',
					'low_range' => 20,
					'high_range' => 40,
					'min_vnum' => 22500,
					'max_vnum' => 22599,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 77,
					'name' => 'Caverns of Thargeld',
					'credits' => 'Karsus',
					'low_range' => 35,
					'high_range' => 45,
					'min_vnum' => 22600,
					'max_vnum' => 22699,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 78,
					'name' => 'The Red Tower',
					'credits' => 'Ceran',
					'low_range' => 35,
					'high_range' => 51,
					'min_vnum' => 23200,
					'max_vnum' => 23229,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 79,
					'name' => 'The Blue Tower',
					'credits' => 'Ceran',
					'low_range' => 35,
					'high_range' => 51,
					'min_vnum' => 23230,
					'max_vnum' => 23259,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 80,
					'name' => 'The Green Tower',
					'credits' => 'Ceran',
					'low_range' => 35,
					'high_range' => 51,
					'min_vnum' => 23260,
					'max_vnum' => 23267,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 81,
					'name' => 'Bounty Cabal',
					'credits' => 'Jathruk/Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 23600,
					'max_vnum' => 23624,
					'type' => 2,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 82,
					'name' => 'The Horde Longhouse',
					'credits' => 'Ktyvan',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 23625,
					'max_vnum' => 23649,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 83,
					'name' => 'The Scion Enclave',
					'credits' => 'Zethus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 23650,
					'max_vnum' => 23666,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 84,
					'name' => 'The Common Guild',
					'credits' => 'Waekath',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 23675,
					'max_vnum' => 23699,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 85,
					'name' => 'The Regal Theatre',
					'credits' => 'Eluviel',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 23700,
					'max_vnum' => 23715,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 86,
					'name' => 'The Glass Tower',
					'credits' => 'Zethus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 23800,
					'max_vnum' => 23875,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 87,
					'name' => 'The Sun Temple',
					'credits' => 'None',
					'low_range' => 0,
					'high_range' => 0,
					'min_vnum' => 23876,
					'max_vnum' => 23889,
					'type' => 5,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 88,
					'name' => 'Dead Forest',
					'credits' => 'Ceran',
					'low_range' => 10,
					'high_range' => 40,
					'min_vnum' => 24200,
					'max_vnum' => 24299,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 89,
					'name' => 'Dying Forest',
					'credits' => 'Ceran',
					'low_range' => 25,
					'high_range' => 51,
					'min_vnum' => 24300,
					'max_vnum' => 24399,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 90,
					'name' => 'Koteras-Udgaard Road',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 24400,
					'max_vnum' => 24429,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 91,
					'name' => 'Melcene-Udgaard Road',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 24430,
					'max_vnum' => 24449,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 92,
					'name' => 'Melcene-Plains Road',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 24450,
					'max_vnum' => 24459,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 93,
					'name' => 'Melcene-Cimar Road',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 24460,
					'max_vnum' => 24499,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 94,
					'name' => 'Iseldheim-Dale Road',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 24500,
					'max_vnum' => 24512,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 95,
					'name' => 'Iseldheim-Varggin Shih Road',
					'credits' => 'Tiernus',
					'low_range' => 1,
					'high_range' => 51,
					'min_vnum' => 24513,
					'max_vnum' => 24524,
					'type' => 0,
					'climate' => 1,
					'flags' => 0,
				],
				[
					'id' => 96,
					'name' => 'The Shalaran Academy',
					'credits' => 'Seikilos',
					'low_range' => 1,
					'high_range' => 10,
					'min_vnum' => 24525,
					'max_vnum' => 24599,
					'type' => 0,
					'climate' => 0,
					'flags' => 0,
				],
				[
					'id' => 97,
					'name' => 'Kobold Mines',
					'credits' => 'Skrije',
					'low_range' => 10,
					'high_range' => 30,
					'min_vnum' => 24600,
					'max_vnum' => 24799,
					'type' => 5,
					'climate' => 7,
					'flags' => 0,
				],
				[
					'id' => 98,
					'name' => 'Brynn Mahar',
					'credits' => 'Amadioru',
					'low_range' => 30,
					'high_range' => 51,
					'min_vnum' => 28500,
					'max_vnum' => 28799,
					'type' => 5,
					'climate' => 1,
					'flags' => 0,
				]
			]);
	}
}
