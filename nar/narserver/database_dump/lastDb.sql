-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema nar
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema nar
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `nar` DEFAULT CHARACTER SET utf8 ;
USE `nar` ;

-- -----------------------------------------------------
-- Table `nar`.`Directories`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`Directories` ;

CREATE TABLE IF NOT EXISTS `nar`.`Directories` (
  `Dir_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Dir_name` VARCHAR(120) NULL DEFAULT NULL,
  `Dir_size` BIGINT UNSIGNED NULL DEFAULT 0,
  `Change_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE INDEX `Dir_Id_UNIQUE` (`Dir_id` ASC),
  PRIMARY KEY (`Dir_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Users`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`Users` ;

CREATE TABLE IF NOT EXISTS `nar`.`Users` (
  `User_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `User_name` VARCHAR(100) NOT NULL,
  `AESCrypted` VARCHAR(10000) NOT NULL,
  `RSAPriCrypted` VARCHAR(10000) NOT NULL,
  `RSAPub` VARCHAR(10000) NOT NULL,
  `Dir_id` BIGINT UNSIGNED NOT NULL,
  `Change_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  UNIQUE INDEX `User_Id_UNIQUE` (`User_id` ASC),
  PRIMARY KEY (`User_id`),
  UNIQUE INDEX `User_name_UNIQUE` (`User_name` ASC),
  INDEX `fk_Users_1_idx` (`Dir_id` ASC),
  CONSTRAINT `fk_Users_1`
    FOREIGN KEY (`Dir_id`)
    REFERENCES `nar`.`Directories` (`Dir_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Machines`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`Machines` ;

CREATE TABLE IF NOT EXISTS `nar`.`Machines` (
  `Machine_id` VARCHAR(150) NOT NULL,
  `Machine_quota` BIGINT NOT NULL DEFAULT 0,
  `Machine_diskSpace` BIGINT NOT NULL DEFAULT 0,
  `Change_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `User_id` BIGINT UNSIGNED NULL,
  PRIMARY KEY (`Machine_id`),
  UNIQUE INDEX `Machine_Id_UNIQUE` (`Machine_id` ASC),
  INDEX `fk_Machines_1_idx` (`User_id` ASC),
  CONSTRAINT `fk_Machines_1`
    FOREIGN KEY (`User_id`)
    REFERENCES `nar`.`Users` (`User_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Files`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`Files` ;

CREATE TABLE IF NOT EXISTS `nar`.`Files` (
  `File_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `File_name` VARCHAR(120) NULL DEFAULT NULL,
  `File_size` BIGINT UNSIGNED NULL DEFAULT 0,
  `Change_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`File_id`),
  UNIQUE INDEX `File_Id_UNIQUE` (`File_id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Chunks`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`Chunks` ;

CREATE TABLE IF NOT EXISTS `nar`.`Chunks` (
  `Chunk_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `File_id` BIGINT UNSIGNED NOT NULL,
  `Chunk_size` BIGINT UNSIGNED NULL DEFAULT 0,
  `Change_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Chunk_id`),
  UNIQUE INDEX `Chunk_id_UNIQUE` (`Chunk_id` ASC),
  INDEX `File_id` (`File_id` ASC),
  CONSTRAINT `File_id`
    FOREIGN KEY (`File_id`)
    REFERENCES `nar`.`Files` (`File_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`ChunkToMachine`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`ChunkToMachine` ;

CREATE TABLE IF NOT EXISTS `nar`.`ChunkToMachine` (
  `Chunk_id` BIGINT UNSIGNED NOT NULL,
  `Machine_id` VARCHAR(150) NOT NULL,
  PRIMARY KEY (`Chunk_id`, `Machine_id`),
  INDEX `Machine_id` (`Machine_id` ASC),
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
DROP TABLE IF EXISTS `nar`.`UserToFile` ;

CREATE TABLE IF NOT EXISTS `nar`.`UserToFile` (
  `User_id` BIGINT UNSIGNED NOT NULL,
  `File_id` BIGINT UNSIGNED NOT NULL,
  PRIMARY KEY (`User_id`, `File_id`),
  INDEX `Files_id` (`File_id` ASC),
  CONSTRAINT `Users_id`
    FOREIGN KEY (`User_id`)
    REFERENCES `nar`.`Users` (`User_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `Files_id`
    FOREIGN KEY (`File_id`)
    REFERENCES `nar`.`Files` (`File_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`DirectoryTo`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`DirectoryTo` ;

CREATE TABLE IF NOT EXISTS `nar`.`DirectoryTo` (
  `Dir_id` BIGINT UNSIGNED NOT NULL,
  `Item_id` BIGINT UNSIGNED NOT NULL,
  `ForD` TINYINT NOT NULL DEFAULT 0,
  PRIMARY KEY (`Dir_id`, `Item_id`, `ForD`),
  CONSTRAINT `Dirs_id`
    FOREIGN KEY (`Dir_id`)
    REFERENCES `nar`.`Directories` (`Dir_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Sessions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `nar`.`Sessions` ;

CREATE TABLE IF NOT EXISTS `nar`.`Sessions` (
  `session_id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `machine_id` VARCHAR(150) NOT NULL,
  `join_time` DATETIME NULL DEFAULT CURRENT_TIMESTAMP,
  `leave_time` DATETIME NULL,
  INDEX `fk_table1_1_idx` (`machine_id` ASC),
  PRIMARY KEY (`session_id`),
  CONSTRAINT `fk_table1_1`
    FOREIGN KEY (`machine_id`)
    REFERENCES `nar`.`Machines` (`Machine_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
exit
