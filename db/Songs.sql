-- MySQL dump 10.13  Distrib 8.0.32, for macos12.6 (x86_64)
--
-- Host: localhost    Database: music
-- ------------------------------------------------------
-- Server version	8.0.32

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Songs`
--

DROP TABLE IF EXISTS `Songs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Songs` (
  `id` int NOT NULL AUTO_INCREMENT,
  `artist` varchar(255) DEFAULT NULL,
  `track` varchar(255) DEFAULT NULL,
  `title` varchar(255) DEFAULT NULL,
  `remixer` varchar(255) DEFAULT NULL,
  `featuring` varchar(255) DEFAULT NULL,
  `filepath` varchar(255) DEFAULT NULL,
  `reSongId` int DEFAULT NULL,
  `albumId` int DEFAULT NULL,
  `rating` int DEFAULT NULL,
  `dateAdded` datetime NOT NULL,
  `comments` text,
  `trashed` tinyint(1) DEFAULT '0',
  `albumPartId` int DEFAULT NULL,
  `bpm` decimal(5,2) DEFAULT NULL,
  `lowQuality` tinyint(1) DEFAULT '0',
  `tonicKey` enum('C','G','D','A','E','B','Gb','Db','Ab','Eb','Bb','F','Am','Em','Bm','Gbm','Dbm','Abm','Ebm','Bbm','Fm','Cm','Gm','Dm') DEFAULT NULL,
  `energy` tinyint DEFAULT NULL,
  `googleSongId` varchar(255) DEFAULT NULL,
  `durationInMs` int DEFAULT NULL,
  `curator` varchar(255) DEFAULT NULL,
  `musicVideoId` int DEFAULT NULL,
  `youtubeId` varchar(255) DEFAULT NULL,
  `bpmLock` tinyint(1) DEFAULT '0',
  `tonicKeyLock` tinyint(1) DEFAULT '0',
  `spotifyId` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `songs_googleSongId_uq` (`googleSongId`),
  UNIQUE KEY `songs_youtubeId_uq` (`youtubeId`),
  UNIQUE KEY `songs_spotifyId_uq` (`spotifyId`),
  KEY `songs_albumId_idx` (`albumId`),
  KEY `songs_reSongId_idx` (`reSongId`),
  KEY `songs_musicVideoId_idx` (`musicVideoId`),
  KEY `songs_albumPartId_idx` (`albumPartId`),
  CONSTRAINT `fk_songs_albumId` FOREIGN KEY (`albumId`) REFERENCES `Albums` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `fk_songs_albumPartId` FOREIGN KEY (`albumPartId`) REFERENCES `AlbumParts` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `fk_songs_musicVideoId` FOREIGN KEY (`musicVideoId`) REFERENCES `MusicVideos` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `fk_songs_reSongId` FOREIGN KEY (`reSongId`) REFERENCES `RESongs` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE=InnoDB AUTO_INCREMENT=20411 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-03-12  1:35:09
