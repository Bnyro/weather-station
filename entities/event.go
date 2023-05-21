package entities

type Event struct {
	ID    uint64 `gorm:"primaryKey" json:"id"`
	Title string `json:"title"`
	Start string `json:"start"`
	End   string `json:"end"`
	Color string `json:"color"`
}
