package main

import (
	"fmt"
	"log"
	"math/rand"
	"reflect"
	"time"

	"gitee.com/GuaikOrg/go-snowflake/snowflake"
	"github.com/chilts/sid"
	"github.com/kjk/betterguid"
	"github.com/oklog/ulid"
	"github.com/rs/xid"
	uuid "github.com/satori/go.uuid"
	"github.com/segmentio/ksuid"
	"github.com/sony/sonyflake"
)

const FOR_LOOP = 100000

func genXid() {
	id := xid.New()
	fmt.Printf("github.com/rs/xid:           %s, len:%d\n", id.String(), len(id.String()))
}

func genKsuid() {
	id := ksuid.New()
	fmt.Printf("github.com/segmentio/ksuid:  %s, len:%d\n", id.String(), len(id.String()))
}

func genBetterGUID() {
	id := betterguid.New()
	fmt.Printf("github.com/kjk/betterguid:   %s, len:%d\n", id, len(id))
}

func genUlid() {
	t := time.Now().UTC()
	entropy := rand.New(rand.NewSource(t.UnixNano()))
	id := ulid.MustNew(ulid.Timestamp(t), entropy)
	fmt.Printf("github.com/oklog/ulid:       %s, len:%d\n", id.String(), len(id.String()))
}

// https://gitee.com/GuaikOrg/go-snowflake
func genSnowflake() {
	flake, err := snowflake.NewSnowflake(int64(0), int64(0))
	if err != nil {
		log.Fatalf("snowflake.NewSnowflake failed with %s\n", err)
	}
	id := flake.NextVal()
	fmt.Printf("gitee.com/GuaikOrg/go-snowflake:%x, type:%s\n", id, reflect.TypeOf(id))
}

func genSonyflake() {
	flake := sonyflake.NewSonyflake(sonyflake.Settings{})
	id, err := flake.NextID()
	if err != nil {
		log.Fatalf("flake.NextID() failed with %s\n", err)
	}
	fmt.Printf("github.com/sony/sonyflake:   %x, type:%s\n", id, reflect.TypeOf(id))
}

func genSid() {
	id := sid.Id()
	fmt.Printf("github.com/chilts/sid:       %s, len:%d\n", id, len(id))
}

func genUUIDv4() {
	id, err := uuid.NewV4()
	if err != nil {
		fmt.Printf("get uuid error [%s]", err)
	}
	fmt.Printf("github.com/satori/go.uuid:   %s, len:%d\n", id, len(id))
}

func testGenXid(n int) {
	t0 := time.Now()
	for i := 0; i < n; i++ {
		_ = xid.New()
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/rs/xid          n:", n, "time:", elapsed)
}

func testGenKsuid(n int) {
	t0 := time.Now()
	for i := 0; i < n; i++ {
		_ = ksuid.New()
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/segmentio/ksuid n:", n, "time:", elapsed)
}

func testGenBetterguid(n int) {
	t0 := time.Now()
	for i := 0; i < n; i++ {
		_ = betterguid.New()
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/kjk/betterguid  n:", n, "time:", elapsed)
}

func testGenUlid(n int) {
	t0 := time.Now()
	for i := 0; i < n; i++ {
		t := time.Now().UTC()
		entropy := rand.New(rand.NewSource(t.UnixNano()))
		_ = ulid.MustNew(ulid.Timestamp(t), entropy)
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/oklog/ulid      n:", n, "time:", elapsed)
}

func testGenSnowflake(n int) {
	t0 := time.Now()
	flake, err := snowflake.NewSnowflake(int64(0), int64(0))
	if err != nil {
		log.Fatalf("snowflake.NewSnowflake failed with %s\n", err)
	}
	for i := 0; i < n; i++ {
		_ = flake.NextVal()
	}
	elapsed := time.Since(t0)
	fmt.Println("gitee.com/GuaikOrg/go-snowflake n:", n, "time:", elapsed)
}
func testGenSonyflake(n int) {
	t0 := time.Now()
	flake := sonyflake.NewSonyflake(sonyflake.Settings{}) // 注意这一行的位置
	for i := 0; i < n; i++ {
		_, err := flake.NextID()
		if err != nil {
			log.Fatalf("flake.NextID() failed with %s\n", err)
		}
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/sony/sonyflake  n:", n, "time:", elapsed)
}

func testGenSid(n int) {
	t0 := time.Now()
	for i := 0; i < n; i++ {
		_ = sid.Id()
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/chilts/sid      n:", n, "time:", elapsed)
}

func testGenUUIDv4(n int) {
	t0 := time.Now()
	for i := 0; i < n; i++ {
		_, err := uuid.NewV4()
		if err != nil {
			fmt.Printf("get uuid error [%s]", err)
		}
	}
	elapsed := time.Since(t0)
	fmt.Println("github.com/satori/go.uuid  n:", n, "time:", elapsed)
}

func main() {
	fmt.Printf("效果展示...\n")
	genXid()
	genXid()
	genXid()
	genKsuid()
	genBetterGUID()
	genUlid()
	genSnowflake()
	genSonyflake()
	genSid()
	genUUIDv4()
	fmt.Printf("性能测试...\n")
	testGenXid(FOR_LOOP)
	testGenKsuid(FOR_LOOP)
	testGenBetterguid(FOR_LOOP)
	testGenUlid(FOR_LOOP)
	testGenSnowflake(FOR_LOOP)
	testGenSonyflake(FOR_LOOP)
	testGenSid(FOR_LOOP)
	testGenUUIDv4(FOR_LOOP)
}

// github.com/rs/xid          n: 1000000 time: 29.2665ms
// github.com/segmentio/ksuid n: 1000000 time: 311.4816ms
// github.com/kjk/betterguid  n: 1000000 time: 89.2803ms
// github.com/oklog/ulid   n: 1000000 time: 11.746259s
// github.com/sony/sonyflake   n: 1000000 time: 39.0713342s
// thub.com/chilts/sid        n: 1000000 time: 254.9442ms
// github.com/satori/go.uuid     n: 1000000 time: 270.3201ms
