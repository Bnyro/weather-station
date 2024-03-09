package config

import (
	"github.com/bnyro/homeserver/entities"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

var Database *gorm.DB

const DATABASE_URI = "postgres://postgres@localhost/homeserver"

func Connect() error {
	var err error

	Database, err = gorm.Open(
		sqlite.Open("homeserver.db"),
		&gorm.Config{
			SkipDefaultTransaction: true,
			PrepareStmt:            true,
		})

	if err != nil {
		panic(err)
	}

	Database.AutoMigrate(&entities.WeatherData{})

	return nil
}
