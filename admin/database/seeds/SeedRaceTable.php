<?php

use Illuminate\Database\Seeder;

class SeedRaceTable extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run()
	{
		// TODO: Uncomment for admin installer
		//DB::connection('rift_core')
		//	->table('race_table')
		//	->insert([
		//		[
		//			'name' => 'unique',
		//			'short_name' => 'unique',
		//			'imm_name' => 'unique',
		//			'race_time' => 'unique',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'human',
		//			'short_name' => ' Human',
		//			'imm_name' => 'Human',
		//			'race_time' => 'human',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 2,
		//			'age_mod' => 1,
		//			'deathtime' => 1,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 20,
		//			'max_int' => 20,
		//			'max_wis' => 20,
		//			'max_dex' => 20,
		//			'max_con' => 20
		//		],
		//		[
		//			'name' => 'dwarf',
		//			'short_name' => ' Dwarf',
		//			'imm_name' => 'Dwarf',
		//			'race_time' => 'dwarven',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 2,
		//			'age_mod' => 3,
		//			'deathtime' => 5,
		//			'align' => 1,
		//			'xp_add' => 250,
		//			'max_str' => 23,
		//			'max_int' => 17,
		//			'max_wis' => 20,
		//			'max_dex' => 17,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'duergar',
		//			'short_name' => ' Duerg',
		//			'imm_name' => 'Duerg',
		//			'race_time' => 'duergar',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 2,
		//			'age_mod' => 3,
		//			'deathtime' => 5,
		//			'align' => 6,
		//			'xp_add' => 127,
		//			'max_str' => 22,
		//			'max_int' => 19,
		//			'max_wis' => 18,
		//			'max_dex' => 20,
		//			'max_con' => 23
		//		],
		//		[
		//			'name' => 'elf',
		//			'short_name' => '  Elf',
		//			'imm_name' => ' Elf',
		//			'race_time' => 'elven',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 2,
		//			'age_mod' => 10,
		//			'deathtime' => 8,
		//			'align' => 4,
		//			'xp_add' => 300,
		//			'max_str' => 17,
		//			'max_int' => 25,
		//			'max_wis' => 21,
		//			'max_dex' => 22,
		//			'max_con' => 16
		//		],
		//		[
		//			'name' => 'drow',
		//			'short_name' => ' Drow',
		//			'imm_name' => 'Drow',
		//			'race_time' => 'drow',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 2,
		//			'age_mod' => 9,
		//			'deathtime' => 7,
		//			'align' => 6,
		//			'xp_add' => 300,
		//			'max_str' => 17,
		//			'max_int' => 24,
		//			'max_wis' => 20,
		//			'max_dex' => 23,
		//			'max_con' => 17
		//		],
		//		[
		//			'name' => 'celestial',
		//			'short_name' => 'Celest',
		//			'imm_name' => 'Celes',
		//			'race_time' => 'titan',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 3,
		//			'age_mod' => 6,
		//			'deathtime' => 6,
		//			'align' => 4,
		//			'xp_add' => 500,
		//			'max_str' => 25,
		//			'max_int' => 18,
		//			'max_wis' => 18,
		//			'max_dex' => 17,
		//			'max_con' => 23
		//		],
		//		[
		//			'name' => 'planar',
		//			'short_name' => 'Planar',
		//			'imm_name' => 'Pla',
		//			'race_time' => 'titan',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 3,
		//			'age_mod' => 6,
		//			'deathtime' => 6,
		//			'align' => 5,
		//			'xp_add' => 500,
		//			'max_str' => 25,
		//			'max_int' => 17,
		//			'max_wis' => 17,
		//			'max_dex' => 18,
		//			'max_con' => 22
		//		],
		//		[
		//			'name' => 'abyss',
		//			'short_name' => ' Abyss',
		//			'imm_name' => 'Abyss',
		//			'race_time' => 'titan',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 3,
		//			'age_mod' => 6,
		//			'deathtime' => 6,
		//			'align' => 6,
		//			'xp_add' => 500,
		//			'max_str' => 25,
		//			'max_int' => 15,
		//			'max_wis' => 16,
		//			'max_dex' => 20,
		//			'max_con' => 23
		//		],
		//		[
		//			'name' => 'sidhe',
		//			'short_name' => ' Sidhe',
		//			'imm_name' => 'Sidhe',
		//			'race_time' => 'sidhe',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 1,
		//			'age_mod' => 0.75,
		//			'deathtime' => 4,
		//			'align' => 4,
		//			'xp_add' => 250,
		//			'max_str' => 17,
		//			'max_int' => 21,
		//			'max_wis' => 25,
		//			'max_dex' => 21,
		//			'max_con' => 18
		//		],
		//		[
		//			'name' => 'halfling',
		//			'short_name' => 'Halfling',
		//			'imm_name' => 'Half',
		//			'race_time' => 'halfling',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 1,
		//			'age_mod' => 5,
		//			'deathtime' => 5,
		//			'align' => 5,
		//			'xp_add' => 250,
		//			'max_str' => 17,
		//			'max_int' => 19,
		//			'max_wis' => 23,
		//			'max_dex' => 25,
		//			'max_con' => 20
		//		],
		//		[
		//			'name' => 'imp',
		//			'short_name' => '  Imp',
		//			'imm_name' => 'Imp',
		//			'race_time' => 'imp',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 1,
		//			'age_mod' => 4,
		//			'deathtime' => 3,
		//			'align' => 6,
		//			'xp_add' => 250,
		//			'max_str' => 17,
		//			'max_int' => 24,
		//			'max_wis' => 22,
		//			'max_dex' => 24,
		//			'max_con' => 16
		//		],
		//		[
		//			'name' => 'minotaur',
		//			'short_name' => ' Mino',
		//			'imm_name' => 'Mino',
		//			'race_time' => 'minotaur',
		//			'pcrace' => 1,
		//			'race_status' => 1,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 2,
		//			'age_mod' => 1,
		//			'deathtime' => 1,
		//			'align' => 0,
		//			'xp_add' => 100,
		//			'max_str' => 24,
		//			'max_int' => 17,
		//			'max_wis' => 18,
		//			'max_dex' => 19,
		//			'max_con' => 22
		//		],
		//		[
		//			'name' => 'avian',
		//			'short_name' => 'avian',
		//			'imm_name' => 'avian',
		//			'race_time' => 'avian',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'bat',
		//			'short_name' => 'bat',
		//			'imm_name' => 'bat',
		//			'race_time' => 'bat',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 0,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'bear',
		//			'short_name' => 'bear',
		//			'imm_name' => 'bear',
		//			'race_time' => 'bear',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'beholder',
		//			'short_name' => 'beholder',
		//			'imm_name' => 'beholder',
		//			'race_time' => 'beholder',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 0,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'cat',
		//			'short_name' => 'cat',
		//			'imm_name' => 'cat',
		//			'race_time' => 'cat',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'centaur',
		//			'short_name' => 'centaur',
		//			'imm_name' => 'centaur',
		//			'race_time' => 'centaur',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'centipede',
		//			'short_name' => 'centipede',
		//			'imm_name' => 'centipede',
		//			'race_time' => 'centipede',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 20,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'demon',
		//			'short_name' => 'demon',
		//			'imm_name' => 'demon',
		//			'race_time' => 'demon',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'dog',
		//			'short_name' => 'dog',
		//			'imm_name' => 'dog',
		//			'race_time' => 'dog',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'dragon',
		//			'short_name' => 'dragon',
		//			'imm_name' => 'dragon',
		//			'race_time' => 'dragon',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'fido',
		//			'short_name' => 'fido',
		//			'imm_name' => 'fido',
		//			'race_time' => 'fido',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'fish',
		//			'short_name' => 'fish',
		//			'imm_name' => 'fish',
		//			'race_time' => 'fish',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 0,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'fox',
		//			'short_name' => 'fox',
		//			'imm_name' => 'fox',
		//			'race_time' => 'fox',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'goblin',
		//			'short_name' => 'goblin',
		//			'imm_name' => 'goblin',
		//			'race_time' => 'goblin',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'grimalkin',
		//			'short_name' => 'grimalkin',
		//			'imm_name' => 'grimalkin',
		//			'race_time' => 'grimalkin',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'hobgoblin',
		//			'short_name' => 'hobgoblin',
		//			'imm_name' => 'hobgoblin',
		//			'race_time' => 'hobgoblin',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'insect',
		//			'short_name' => 'insect',
		//			'imm_name' => 'insect',
		//			'race_time' => 'insect',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 6,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'kobold',
		//			'short_name' => 'kobold',
		//			'imm_name' => 'kobold',
		//			'race_time' => 'kobold',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'lich',
		//			'short_name' => 'lich',
		//			'imm_name' => 'lich',
		//			'race_time' => 'lich',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'lizard',
		//			'short_name' => 'lizard',
		//			'imm_name' => 'lizard',
		//			'race_time' => 'lizard',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'pig',
		//			'short_name' => 'pig',
		//			'imm_name' => 'pig',
		//			'race_time' => 'pig',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'rabbit',
		//			'short_name' => 'rabbit',
		//			'imm_name' => 'rabbit',
		//			'race_time' => 'rabbit',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'rakshasa',
		//			'short_name' => 'rakshasa',
		//			'imm_name' => 'rakshasa',
		//			'race_time' => 'rakshasa',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'rat',
		//			'short_name' => 'rat',
		//			'imm_name' => 'rat',
		//			'race_time' => 'rat',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'skeleton',
		//			'short_name' => 'skeleton',
		//			'imm_name' => 'skeleton',
		//			'race_time' => 'skeleton',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'snake',
		//			'short_name' => 'snake',
		//			'imm_name' => 'snake',
		//			'race_time' => 'snake',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 0,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'spectre',
		//			'short_name' => 'spectre',
		//			'imm_name' => 'spectre',
		//			'race_time' => 'spectre',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 0,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'spirit',
		//			'short_name' => 'spirit',
		//			'imm_name' => 'spirit',
		//			'race_time' => 'spirit',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 0,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'troll',
		//			'short_name' => 'troll',
		//			'imm_name' => 'troll',
		//			'race_time' => 'troll',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'vampire',
		//			'short_name' => 'vampire',
		//			'imm_name' => 'vampire',
		//			'race_time' => 'vampire',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'water fowl',
		//			'short_name' => 'water fowl',
		//			'imm_name' => 'water fowl',
		//			'race_time' => 'water fowl',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'wolf',
		//			'short_name' => 'wolf',
		//			'imm_name' => 'wolf',
		//			'race_time' => 'wolf',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'wyvern',
		//			'short_name' => 'wyvern',
		//			'imm_name' => 'wyvern',
		//			'race_time' => 'wyvern',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 0,
		//			'legs' => 4,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		],
		//		[
		//			'name' => 'zombie',
		//			'short_name' => 'zombie',
		//			'imm_name' => 'zombie',
		//			'race_time' => 'zombie',
		//			'pcrace' => 0,
		//			'race_status' => 0,
		//			'arms' => 2,
		//			'legs' => 2,
		//			'size' => 0,
		//			'age_mod' => 0,
		//			'deathtime' => 0,
		//			'align' => 0,
		//			'xp_add' => 0,
		//			'max_str' => 25,
		//			'max_int' => 25,
		//			'max_wis' => 25,
		//			'max_dex' => 25,
		//			'max_con' => 25
		//		]
		//	]);
	}
}
