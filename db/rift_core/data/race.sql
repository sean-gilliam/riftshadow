
--
-- Dumping data for table `race_table`
--

LOCK TABLES `race_table` WRITE;
/*!40000 ALTER TABLE `race_table` DISABLE KEYS */;
INSERT INTO `race_table` VALUES (1,'unique','unique','unique','unique',0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,25,25,25,25,25),(2,'human',' Human','Human','human',1,1,2048,0,0,0,0,0,2101377,2047,2,2,2,1,1,0,0,20,20,20,20,20),(3,'dwarf',' Dwarf','Dwarf','dwarven',1,1,2048,33554432,0,0,67584,0,2101377,2047,2,2,2,3,5,1,250,23,17,20,17,25),(4,'duergar',' Duerg','Duerg','duergar',1,1,2048,33554432,0,0,16,262144,2101377,2047,2,2,2,3,5,6,127,22,19,18,20,23),(5,'elf','  Elf',' Elf','elven',1,1,2048,33554432,0,0,32770,33554432,2101377,2047,2,2,2,10,8,4,300,17,25,21,22,16),(6,'drow',' Drow','Drow','drow',1,1,2048,33554432,0,0,32770,8388608,2101377,2047,2,2,2,9,7,6,300,17,24,20,23,17),(7,'celestial','Celest','Celes','titan',1,1,2048,0,0,0,8200,4096,2101377,2047,2,2,3,6,6,4,500,25,18,18,17,23),(8,'planar','Planar','Pla','titan',1,1,2048,0,0,0,520,67584,2101377,2047,2,2,3,6,6,5,500,25,17,17,18,22),(9,'abyss',' Abyss','Abyss','titan',1,1,2048,0,0,0,4232,41216,2101377,2047,2,2,3,6,6,6,500,25,15,16,20,23),(10,'sidhe',' Sidhe','Sidhe','sidhe',1,1,2048,524288,0,0,256,16,2101377,2047,2,2,1,0.75,4,4,250,17,21,25,21,18),(11,'halfling','Halfling','Half','halfling',1,1,2048,32768,0,0,0,16,2101377,2047,2,2,1,5,5,5,250,17,19,23,25,20),(12,'imp','  Imp','Imp','imp',1,1,2048,524288,0,0,4,512,2101377,2047,2,2,1,4,3,6,250,17,24,22,24,16),(13,'minotaur',' Mino','Mino','minotaur',1,1,2048,0,0,0,256,128,2101377,4196351,2,2,2,1,1,0,100,24,17,18,19,22),(14,'avian','avian','avian','avian',0,0,0,524288,0,0,0,131584,2101377,34815,2,2,0,0,0,0,0,25,25,25,25,25),(15,'bat','bat','bat','bat',0,0,0,34078720,160,0,0,262144,2097217,34493,0,0,0,0,0,0,0,25,25,25,25,25),(16,'bear','bear','bear','bear',0,0,0,0,160,0,272,262144,2097217,3147453,2,2,0,0,0,0,0,25,25,25,25,25),(17,'beholder','beholder','beholder','beholder',0,0,2048,34078720,0,67272704,8,0,132,16782336,0,0,0,0,0,0,0,25,25,25,25,25),(18,'cat','cat','cat','cat',0,0,0,33554432,160,0,0,0,2097217,3212989,0,4,0,0,0,0,0,25,25,25,25,25),(19,'centaur','centaur','centaur','centaur',0,0,2048,0,0,0,18,32768,2101377,2047,2,4,0,0,0,0,0,25,25,25,25,25),(20,'centipede','centipede','centipede','centipede',0,0,0,33554432,0,0,288,16,16451,1029,0,20,0,0,0,0,0,25,25,25,25,25),(21,'demon','demon','demon','demon',0,0,2048,33554432,8228,67175424,4224,0,2101377,16779263,2,2,0,0,0,0,0,25,25,25,25,25),(22,'dog','dog','dog','dog',0,0,0,0,128,0,0,0,2097217,3147453,0,4,0,0,0,0,0,25,25,25,25,25),(23,'dragon','dragon','dragon','dragon',0,0,2048,34078720,4096,0,18,0,33554561,8471593,0,4,0,0,0,0,0,25,25,25,25,25),(24,'fido','fido','fido','fido',0,0,0,0,128,0,0,4,2097219,2164413,0,4,0,0,0,0,0,25,25,25,25,25),(25,'fish','fish','fish','fish',0,0,0,134217728,32,0,3,0,134225985,8471593,0,0,0,0,0,0,0,25,25,25,25,25),(26,'fox','fox','fox','fox',0,0,0,33554432,160,0,0,0,2097217,2164413,0,4,0,0,0,0,0,25,25,25,25,25),(27,'goblin','goblin','goblin','goblin',0,0,0,33554432,0,0,65536,4,2101377,2047,2,2,0,0,0,0,0,25,25,25,25,25),(28,'grimalkin','grimalkin','grimalkin','grimalkin',0,0,0,33554432,0,0,262148,131200,2101377,2047,2,2,0,0,0,0,0,25,25,25,25,25),(29,'hobgoblin','hobgoblin','hobgoblin','hobgoblin',0,0,0,33554432,0,0,67584,0,2047,16779263,2,2,0,0,0,0,0,25,25,25,25,25),(30,'insect','insect','insect','insect',0,0,0,0,0,0,2048,128,16385,1063,0,6,0,0,0,0,0,25,25,25,25,25),(31,'kobold','kobold','kobold','kobold',0,0,0,33554432,0,0,2048,4,2101379,67583,2,2,0,0,0,0,0,25,25,25,25,25),(32,'lich','lich','lich','lich',0,0,16384,33554440,0,67344384,2304,16785536,257,0,2,2,0,0,0,0,0,25,25,25,25,25),(33,'lizard','lizard','lizard','lizard',0,0,0,0,0,0,2048,256,8388673,1725,0,4,0,0,0,0,0,25,25,25,25,25),(34,'pig','pig','pig','pig',0,0,0,0,0,0,0,0,2097217,1725,0,4,0,0,0,0,0,25,25,25,25,25),(35,'rabbit','rabbit','rabbit','rabbit',0,0,0,0,160,0,0,0,2097217,1725,0,4,0,0,0,0,0,25,25,25,25,25),(36,'rakshasa','rakshasa','rakshasa','rakshasa',0,0,0,33554440,0,0,12288,128,129,3147247,2,2,0,0,0,0,0,25,25,25,25,25),(37,'rat','rat','rat','rat',0,0,0,33554440,0,0,12288,128,129,3147247,0,4,0,0,0,0,0,25,25,25,25,25),(38,'skeleton','skeleton','skeleton','skeleton',0,0,16384,33554432,512,67344384,32,8336,257,199,2,2,0,0,0,0,0,25,25,25,25,25),(39,'snake','snake','snake','snake',0,0,0,0,0,0,2048,256,25165889,10554425,0,0,0,0,0,0,0,25,25,25,25,25),(40,'spectre','spectre','spectre','spectre',0,0,16384,35127296,0,67344384,264,8320,257,0,2,0,0,0,0,0,0,25,25,25,25,25),(41,'spirit','spirit','spirit','spirit',0,0,16384,35127296,0,67344384,264,8320,257,0,2,0,0,0,0,0,0,25,25,25,25,25),(42,'troll','troll','troll','troll',0,0,0,0,0,0,67592,1152,2101377,2047,2,2,0,0,0,0,0,25,25,25,25,25),(43,'vampire','vampire','vampire','vampire',0,0,16384,33554432,0,67344384,0,8352,257,199,2,2,0,0,0,0,0,25,25,25,25,25),(44,'water fowl','water fowl','water fowl','water fowl',0,0,0,134742016,160,0,131072,0,4194369,33981,0,2,0,0,0,0,0,25,25,25,25,25),(45,'wolf','wolf','wolf','wolf',0,0,0,33554432,160,0,0,0,2097217,2164285,0,4,0,0,0,0,0,25,25,25,25,25),(46,'wyvern','wyvern','wyvern','wyvern',0,0,0,524328,164,2048,0,262144,33554499,10553021,0,4,0,0,0,0,0,25,25,25,25,25),(47,'zombie','zombie','zombie','zombie',0,0,16384,33554432,0,67344384,0,8320,257,199,2,2,0,0,0,0,0,25,25,25,25,25);
/*!40000 ALTER TABLE `race_table` ENABLE KEYS */;
UNLOCK TABLES;