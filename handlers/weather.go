package handlers

import (
	"encoding/json"
	"fmt"
	"net/http"
	"time"

	"github.com/bnyro/homeserver/config"
	"github.com/bnyro/homeserver/entities"
	"github.com/bnyro/homeserver/utilities"
)

func GetWeatherData(w http.ResponseWriter, r *http.Request) {
	var data []entities.WeatherData

	config.Database.Find(&data)

	utilities.WriteJsonStatus(w, http.StatusOK, data)
}

func CreateWeatherData(w http.ResponseWriter, r *http.Request) {
	weatherData := new(entities.WeatherData)

	if err := json.NewDecoder(r.Body).Decode(&weatherData); err != nil {
		utilities.WriteJsonStatus(w, http.StatusBadRequest, "Bad Request")
		return
	}
	weatherData.Time = time.Now().Unix()

	config.Database.Create(&weatherData)

	utilities.WriteJson(w, weatherData)
}

func DeleteWeatherData(w http.ResponseWriter, r *http.Request) {
	config.Database.Where("1 = 1").Delete(&entities.WeatherData{})

	utilities.WriteJson(w, entities.Message{
		Message: "Succesfully deleted all!",
	})
}

func LatestWeather(w http.ResponseWriter, r *http.Request) {
	weatherData := new(entities.WeatherData)
	config.Database.Last(weatherData)

	format := r.URL.Query().Get("format")

	if format != "csv" {
		utilities.WriteJson(w, weatherData)
	} else {
		date := time.Unix(weatherData.Time, 0).Format(time.RFC822)
		body := fmt.Sprintf("%s,%0.1f,%0.1f,%0.f", date, weatherData.Ds18_t, weatherData.Dht_h, weatherData.Bmp_p)
		w.Write([]byte(body))
	}
}
