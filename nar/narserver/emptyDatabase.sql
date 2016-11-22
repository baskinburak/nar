SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `nar` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `nar` ;

-- -----------------------------------------------------
-- Table `nar`.`Users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`Users` (
  `User_id` BIGINT UNSIGNED NOT NULL,
  `User_name` VARCHAR(100) NOT NULL,
  `Quota` BIGINT UNSIGNED NOT NULL,
  `Disk_space` BIGINT UNSIGNED NOT NULL,
  `CryptedKey` VARCHAR(10000) NOT NULL,
  UNIQUE INDEX `User_Id_UNIQUE` (`User_id` ASC),
  PRIMARY KEY (`User_id`),
  UNIQUE INDEX `User_name_UNIQUE` (`User_name` ASC),
  UNIQUE INDEX `CryptedKey_UNIQUE` (`CryptedKey` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Machines`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`Machines` (
  `Machine_id` VARCHAR(1000) NOT NULL,
  `User_id` BIGINT UNSIGNED NOT NULL,
  `Machine_quota` BIGINT NOT NULL,
  `Machine_diskSpace` BIGINT NOT NULL,
  PRIMARY KEY (`Machine_id`),
  UNIQUE INDEX `Machine_Id_UNIQUE` (`Machine_id` ASC),
  UNIQUE INDEX `User_Id_UNIQUE` (`User_id` ASC),
  CONSTRAINT `User_id`
    FOREIGN KEY (`User_id`)
    REFERENCES `nar`.`Users` (`User_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Files`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`Files` (
  `File_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `File_name` VARCHAR(120) NULL,
  `File_size` VARCHAR(45) NULL,
  `File_type` VARCHAR(45) NULL,
  PRIMARY KEY (`File_id`),
  UNIQUE INDEX `File_Id_UNIQUE` (`File_id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Chunks`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`Chunks` (
  `Chunk_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `File_id` BIGINT UNSIGNED NOT NULL,
  `Chunk_size` VARCHAR(45) NULL,
  PRIMARY KEY (`Chunk_id`),
  UNIQUE INDEX `Chunk_id_UNIQUE` (`Chunk_id` ASC),
  UNIQUE INDEX `File_id_UNIQUE` (`File_id` ASC),
  CONSTRAINT `File_id`
    FOREIGN KEY (`File_id`)
    REFERENCES `nar`.`Files` (`File_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`ChunkToMachine`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`ChunkToMachine` (
  `Chunk_id` BIGINT UNSIGNED NOT NULL,
  `Machine_id` VARCHAR(1000) NOT NULL,
  UNIQUE INDEX `Peer_id_UNIQUE` (`Machine_id` ASC),
  UNIQUE INDEX `Chunk_id_UNIQUE` (`Chunk_id` ASC),
  PRIMARY KEY (`Chunk_id`, `Machine_id`),
  CONSTRAINT `Chunk_id`
    FOREIGN KEY (`Chunk_id`)
    REFERENCES `nar`.`Chunks` (`Chunk_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `Machine_id`
    FOREIGN KEY (`Machine_id`)
    REFERENCES `nar`.`Machines` (`Machine_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`UserToFile`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`UserToFile` (
  `User_id` BIGINT UNSIGNED NOT NULL,
  `File_id` BIGINT UNSIGNED NOT NULL,
  PRIMARY KEY (`User_id`, `File_id`),
  UNIQUE INDEX `User_id_UNIQUE` (`User_id` ASC),
  UNIQUE INDEX `File_id_UNIQUE` (`File_id` ASC),
  CONSTRAINT `User_id`
    FOREIGN KEY (`User_id`)
    REFERENCES `nar`.`Users` (`User_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `File_id`
    FOREIGN KEY (`File_id`)
    REFERENCES `nar`.`Files` (`File_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
