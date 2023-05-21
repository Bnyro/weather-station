package entities

type WeatherData struct {
	ID     uint    `gorm:"primarykey" json:"id"`
	Time   int64   `json:"time"`
	Ds18_t float32 `json:"ds18_t"`
	Dht_t  float32 `json:"dht_t"`
	Dht_h  float32 `json:"dht_h"`
	Bmp_t  float32 `json:"bmp_t"`
	Bmp_p  float32 `json:"bmp_p"`
}
