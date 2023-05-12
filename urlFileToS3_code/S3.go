package main

import (
	"fmt"
	"os"

	"github.com/aws/aws-sdk-go/aws"
	"github.com/aws/aws-sdk-go/aws/credentials"
	"github.com/aws/aws-sdk-go/aws/session"
	"github.com/aws/aws-sdk-go/service/s3"
)

//export GO_ROOT=/usr/local/go1.16/go
const (
	ACCESS_KEY_ID     = "RNHAVVGGFENTI5RNJ9TY"
	ACCESS_KEY_SECRET = "nu73Ro7U8GYWoSXkd28tcg8zNhQoKoELzvDsa6XB"
	REGION            = "shanghai"
	ENDPOINT          = "http://shanghai.xstore.qihu.com"
	//BUCKET_NAME       = ""
	//FILE_NAME         = ""
)

func sends3(FILE_NAME, BUCKET_NAME string) {
	file1, _ := os.OpenFile(FILE_NAME, os.O_RDONLY, os.ModeType)
	cr := credentials.NewStaticCredentials(ACCESS_KEY_ID, ACCESS_KEY_SECRET, "")
	sess, _ := session.NewSession(&aws.Config{
		Region:           aws.String(REGION),
		Endpoint:         aws.String(ENDPOINT),
		LogLevel:         aws.LogLevel(aws.LogDebugWithSigning),
		S3ForcePathStyle: aws.Bool(true),
		//S3UseAccelerate:  aws.Bool(false),
		Credentials: cr,
	})
	svc := s3.New(sess)
	//key := fmt.Sprintf("%d%s", time.Now().UnixNano(), path.Ext(FILE_NAME))
	key := FILE_NAME
	result, err := svc.PutObject((&s3.PutObjectInput{}).
		SetBucket(BUCKET_NAME).
		SetKey(key).
		SetBody(file1),
	) //这里面会打印HTTP的reqeust和response的详细信息
	if err != nil {
		fmt.Println(result, err) //{ETag: "2dd4e342b7274dd8bb825dfd939c1576"}
	}
	//wg.Done()
}

func getfroms3(FILE_NAME, BUCKET_NAME string) bool {
	cr := credentials.NewStaticCredentials(ACCESS_KEY_ID, ACCESS_KEY_SECRET, "")
	sess, _ := session.NewSession(&aws.Config{
		Region:           aws.String(REGION),
		Endpoint:         aws.String(ENDPOINT),
		LogLevel:         aws.LogLevel(aws.LogDebugWithSigning),
		S3ForcePathStyle: aws.Bool(true),
		//S3UseAccelerate:  aws.Bool(false),
		Credentials: cr,
	})
	svc := s3.New(sess)
	//key := fmt.Sprintf("%d%s", time.Now().UnixNano(), path.Ext(FILE_NAME))
	key := FILE_NAME
	result, err := svc.GetObject((&s3.GetObjectInput{}).
		SetBucket(BUCKET_NAME).
		SetKey(key),
	) //这里面会打印HTTP的reqeust和response的详细信息
	//fmt.Println("test")
	//fmt.Println(result, err)
	//打印出来的数据如下
	// {
	// 	AcceptRanges: "bytes",
	// 	Body: buffer(0xc0004dc040),
	// 	ContentLength: 2241215,
	// 	ContentType: "application/vnd.openxmlformats-officedocument.presentationml.presentation",
	// 	ETag: "e3bf95067c516a8ad5d09f2d8f8e28da",
	// 	LastModified: 2023-05-10 07:39:37 +0000 UTC
	//   }
	if err != nil {
		fmt.Println(result, err)
		return false
	}
	return true
	//wg.Done()
}
