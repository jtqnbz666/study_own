'''
CREATE TABLE `users` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(64) DEFAULT NULL,
  `telephone` varchar(12) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4;
'''

'''
 gin_restful  tree
.
├── api
│   └── users.go
├── db
│   └── mysql.go
├── main.go
├── models
│   └── users.go
└── router.go
'''