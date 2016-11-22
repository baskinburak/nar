SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `nar` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `nar` ;

-- -----------------------------------------------------
-- Table `nar`.`User`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`User` (
  `User_id` BIGINT UNSIGNED NOT NULL,
  `User_name` VARCHAR(100) NOT NULL,
  `Quota` BIGINT UNSIGNED NOT NULL,
  `Disk_space` BIGINT UNSIGNED NOT NULL,
  `CryptedKey` VARCHAR(10000) NOT NULL,
  UNIQUE INDEX `User_Id_UNIQUE` (`User_id` ASC),
  PRIMARY KEY (`User_id`),
  UNIQUE INDEX `User_name_UNIQUE` (`User_name` ASC))
ENGINE=InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`Machine`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`Machine` (
  `Machine_id` VARCHAR(150) NOT NULL,
  `User_id` BIGINT UNSIGNED NOT NULL,
  `Machine_quota` BIGINT NOT NULL,
  `Machine_diskSpace` BIGINT NOT NULL,
  PRIMARY KEY (`Machine_id`),
  UNIQUE INDEX `Machine_Id_UNIQUE` (`Machine_id` ASC),
  UNIQUE INDEX `User_Id_UNIQUE` (`User_id` ASC),
  CONSTRAINT `User_id`
    FOREIGN KEY (`User_id`)
    REFERENCES `nar`.`User` (`User_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE=InnoDB;


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
ENGINE=InnoDB;


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
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE=InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`ChunkToPeer`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`ChunkToPeer` (
  `Chunk_id` BIGINT UNSIGNED NOT NULL,
  `Machine_id` VARCHAR(150) NOT NULL,
  UNIQUE INDEX `Peer_id_UNIQUE` (`Machine_id` ASC),
  UNIQUE INDEX `Chunk_id_UNIQUE` (`Chunk_id` ASC),
  PRIMARY KEY (`Chunk_id`, `Machine_id`),
  CONSTRAINT `Chunk_id`
    FOREIGN KEY (`Chunk_id`)
    REFERENCES `nar`.`Chunks` (`Chunk_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `Machine_id`
    FOREIGN KEY (`Machine_id`)
    REFERENCES `nar`.`Machine` (`Machine_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE=InnoDB;


-- -----------------------------------------------------
-- Table `nar`.`UserToFiles`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nar`.`UserToFiles` (
  `User_id` BIGINT UNSIGNED NOT NULL,
  `File_id` BIGINT UNSIGNED NOT NULL,
  PRIMARY KEY (`User_id`, `File_id`),
  UNIQUE INDEX `User_id_UNIQUE` (`User_id` ASC),
  UNIQUE INDEX `File_id_UNIQUE` (`File_id` ASC),
  CONSTRAINT `User_ids`
    FOREIGN KEY (`User_id`)
    REFERENCES `nar`.`User` (`User_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `File_ids`
    FOREIGN KEY (`File_id`)
    REFERENCES `nar`.`Files` (`File_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE=InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;