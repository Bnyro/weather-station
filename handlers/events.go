package handlers

import (
	"encoding/json"
	"net/http"

	"github.com/bnyro/homeserver/config"
	"github.com/bnyro/homeserver/entities"
	"github.com/bnyro/homeserver/utilities"
)

func GetEvents(w http.ResponseWriter, r *http.Request) {
	var events []entities.Event

	config.Database.Find(&events)

	utilities.WriteJson(w, &events)
}

func CreateEvent(w http.ResponseWriter, r *http.Request) {
	event := new(entities.Event)

	if err := json.NewDecoder(r.Body).Decode(&event); err != nil {
		utilities.WriteJsonStatus(w, http.StatusBadRequest, entities.Message{
			Message: "Bad Request",
		})
		return
	}

	config.Database.Create(&event)

	utilities.WriteJson(w, event)
}

func DeleteEvent(w http.ResponseWriter, r *http.Request) {
	event := new(entities.Event)

	if err := json.NewDecoder(r.Body).Decode(&event); err != nil {
		utilities.WriteJsonStatus(w, http.StatusBadRequest, entities.Message{
			Message: "Bad Request",
		})
		return
	}
	result := config.Database.Delete(&entities.Event{}, event.ID)

	if result.RowsAffected == 0 {
		utilities.WriteJsonStatus(w, http.StatusBadRequest, entities.Message{
			Message: "Bad Request",
		})
		return
	}

	utilities.WriteJsonStatus(w, http.StatusAccepted, entities.Message{
		Message: "Deleted succesfully!",
	})
}
