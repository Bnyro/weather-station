package main

import (
	"fmt"
	"log"
	"net/http"

	"github.com/bnyro/homeserver/config"
	"github.com/bnyro/homeserver/entities"
	"github.com/bnyro/homeserver/handlers"
	"github.com/bnyro/homeserver/utilities"
	"github.com/go-chi/chi/v5"
	"github.com/go-chi/cors"
)

func main() {
	config.Connect()

	app := chi.NewRouter()

	app.Use(cors.Handler(
		cors.Options{
			AllowedOrigins: []string{"*"},
		},
	))

	fs := http.FileServer(http.Dir("./frontend"))
	app.Handle("/*", http.StripPrefix("/", fs))

	app.Route("/api", func(api chi.Router) {
		api.Get("/", func(w http.ResponseWriter, r *http.Request) {
			utilities.WriteJsonStatus(w, http.StatusOK, entities.Message{
				Message: "API online.",
			})
		})

		api.Get("/weather", handlers.GetWeatherData)
		api.Post("/weather", handlers.CreateWeatherData)
		api.Delete("/weather", handlers.DeleteWeatherData)
		api.Delete("/weather/latest", handlers.LatestWeather)
	})

	fmt.Println("Listening on http://localhost:8000")
	log.Fatal(http.ListenAndServe(":8000", app))
}
