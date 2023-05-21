package utilities

import (
	"encoding/json"
	"net/http"
)

type Map map[string]interface{}

func WriteJsonStatus(w http.ResponseWriter, statusCode int, data interface{}) {
	w.Header().Add("Content-Type", "application/json")
	w.WriteHeader(statusCode)
	body, _ := json.Marshal(data)
	w.Write(body)
}

func WriteJson(w http.ResponseWriter, data interface{}) {
	WriteJsonStatus(w, http.StatusOK, data)
}
