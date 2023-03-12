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
-- Table structure for table `RESongs`
--

DROP TABLE IF EXISTS `RESongs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `RESongs` (
  `id` int NOT NULL,
  `songidWinfo` varchar(2047) DEFAULT NULL,
  `songid` varchar(2047) DEFAULT NULL,
  `shortid` varchar(2047) DEFAULT NULL,
  `shortidWinfo` varchar(2047) DEFAULT NULL,
  `artist` varchar(255) DEFAULT NULL,
  `album` varchar(2047) DEFAULT NULL,
  `track` varchar(255) DEFAULT NULL,
  `title` varchar(255) DEFAULT NULL,
  `time` varchar(15) DEFAULT NULL,
  `timeSignature` varchar(15) DEFAULT NULL,
  `filename` varchar(2047) DEFAULT NULL,
  `digitalOnly` varchar(3) DEFAULT NULL,
  `compilation` varchar(3) DEFAULT NULL,
  `keyStart` varchar(15) DEFAULT NULL,
  `keyAccuracy` int DEFAULT NULL,
  `bpmStart` decimal(5,2) DEFAULT NULL,
  `bpmAccuracy` int DEFAULT NULL,
  `rating` tinyint DEFAULT NULL,
  `dateAdded` varchar(31) DEFAULT NULL,
  `catalogId` varchar(31) DEFAULT NULL,
  `label` varchar(63) DEFAULT NULL,
  `remix` varchar(255) DEFAULT NULL,
  `numPlays` int DEFAULT NULL,
  `comments` text,
  `releaseDate` varchar(15) DEFAULT NULL,
  `featuring` varchar(255) DEFAULT NULL,
  `keyEnd` varchar(255) DEFAULT NULL,
  `disabled` varchar(3) DEFAULT NULL,
  `bpmEnd` decimal(5,2) DEFAULT NULL,
  `beatIntensity` int DEFAULT NULL,
  `replayGain` decimal(5,2) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-03-12  1:35:08
