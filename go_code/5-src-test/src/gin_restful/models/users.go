package models

import (
	"gin_http/db"
	"log"
)

type Person struct {
	Id        int    `json:"id" form:"id"`
	Name      string `json:"name" form:"name"`
	Telephone string `json:"telephone" form:"telephone"`
}

// 插入
func (person *Person) Create() int64 {
	rs, err := db.SqlDB.Exec("INSERT into users (name, telephone) value (?,?)", person.Name, person.Telephone)
	if err != nil {
		log.Fatal(err)
	}
	id, err := rs.LastInsertId()
	if err != nil {
		log.Fatal(err)
	}
	return id
}

// 查询一条记录
func (p *Person) GetRow() (person Person, err error) {
	person = Person{}
	err = db.SqlDB.QueryRow("select id,name,telephone from users where id = ?", p.Id).Scan(&person.Id, &person.Name, &person.Telephone)
	return
}

// 查询所有记录
func (person *Person) GetRows() (persons []Person, err error) {
	rows, err := db.SqlDB.Query("select id,name,telephone from users")
	for rows.Next() {
		person := Person{}
		err := rows.Scan(&person.Id, &person.Name, &person.Telephone)
		if err != nil {
			log.Fatal(err)
		}
		persons = append(persons, person)
	}
	rows.Close()
	return
}

// 修改
func (person *Person) Update() int64 {
	rs, err := db.SqlDB.Exec("update users set telephone = ? where id = ?", person.Telephone, person.Id)
	if err != nil {
		log.Fatal(err)
	}
	rows, err := rs.RowsAffected()
	if err != nil {
		log.Fatal(err)
	}
	return rows
}

// 删除一条记录
func Delete(id int) int64 {
	rs, err := db.SqlDB.Exec("delete from users where id = ?", id)
	if err != nil {
		log.Fatal()
	}
	rows, err := rs.RowsAffected()
	if err != nil {
		log.Fatal()
	}
	return rows
}
