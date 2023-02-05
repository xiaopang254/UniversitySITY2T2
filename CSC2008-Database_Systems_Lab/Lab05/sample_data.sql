CREATE TABLE book (
title VARCHAR(256) NOT NULL,
format CHAR(9) CHECK(format = 'paperback' OR format='hardcover'),
pages INT,
language VARCHAR(32),
authors VARCHAR(256),
publisher VARCHAR(64),
year DATE,
ISBN10 CHAR(10) NOT NULL UNIQUE,
ISBN13 CHAR(14) PRIMARY KEY
);

CREATE TABLE student (
name VARCHAR(32) NOT NULL,
email VARCHAR(128) PRIMARY KEY,
year DATE NOT NULL,
faculty VARCHAR(62) NOT NULL,
department VARCHAR(32) NOT NULL,
graduate DATE,
CHECK(graduate >= year)
);

CREATE TABLE copy (
owner VARCHAR(128) REFERENCES student(email) ON DELETE CASCADE,
book CHAR(14) REFERENCES book(ISBN13),
copy INT CHECK(copy>0),
available VARCHAR(6) CHECK(available = 'TRUE' OR available='FALSE'),
PRIMARY KEY (owner, book, copy)
);

CREATE TABLE loan (
borrower VARCHAR(128) REFERENCES student(email),
owner VARCHAR(128),
book CHAR(14),
copy INT,
borrowed DATE,
returned DATE,
FOREIGN KEY (owner, book, copy) REFERENCES copy(owner, book, copy) ON DELETE CASCADE,
PRIMARY KEY (borrowed, borrower, owner, book, copy),
CHECK(returned >= borrowed)
);

Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('GOH HUI YING','gohhuiying1989@gmail.com','2008-01-01','ICT','Software Engineering','2012-01-01');
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('TAY WEI GUO','tayweiguo1989@msn.com','2010-08-01','Physical Science and Engineering','CE','2013-01-01');
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('PENG JIAYUAN','pengjiayuan2011@hotmail.com','2008-01-01','ICT','Software Engineering',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('HUANG ZHANPENG','huangzhanpeng1992@msn.com','2010-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHENG ZHEMIN','zhengzhemin1991@yahoo.com','2008-08-01','Faculty of Arts and Social Science','History',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('LIU ZHANPENG','liuzhanpeng2011@msn.com','2007-08-01','ICT','Chemistry',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('HUANG WENXIN','huangwenxin2010@msn.com','2008-01-01','ICT','Math','2014-01-01');
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('WANG NA','wangna1990@yahoo.com','2010-01-01','Physical Science and Engineering','EE',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('NG ANH QUANG','nganhquang1991@yahoo.com','2009-08-01','Physical Science and Engineering','ME','2014-08-01');
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('POH HUI LING','pohhuiling1992@hotmail.com','2007-01-01','ICT','Software Engineering',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('NG QI YANG','ngqiyang1989@msn.com','2009-01-01','Physical Science and Engineering','CE',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('NEELAM DEOL','neelamdeol2011@hotmail.com','2008-01-01','Faculty of Arts and Social Science','Language',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('LIU SHAOJUN','liushaojun2010@msn.com','2007-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('QIN YIYANG','qinyiyang2010@hotmail.com','2007-01-01','health science','CS','2014-08-01');
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('TAY YONG MING','tayyongming1989@gmail.com','2009-01-01','Physical Science and Engineering','CE',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('SIOW CAO KHOA','siowcaokhoa1991@msn.com','2010-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('CHOY YI TING','choyyiting1992@hotmail.com','2009-08-01','ICT','Software Engineering',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('DENNIS BECKHAM','dennisbeckham1989@msn.com','2010-08-01','health science','IS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('FENG MENG','fengmeng1990@gmail.com','2007-01-01','ICT','Software Engineering',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHANG HAN','zhanghan1989@hotmail.com','2008-01-01','Faculty of Arts and Social Science','History',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('JENNY HUNT','jennyhunt1991@gmail.com','2008-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHANG HONG','zhanghong2011@msn.com','2010-08-01','health science','IS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('SEAH TECK KEE','seahteckkee1990@gmail.com','2008-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('LIU YIHUI','liuyihui1990@hotmail.com','2008-01-01','ICT','Software Engineering',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHU CHANG','zhuchang2010@gmail.com','2009-01-01','Faculty of Arts and Social Science','History',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHENG XI','zhengxi1990@yahoo.com','2010-01-01','health science','IS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ANNIE CHAPMAN','anniechapman1991@yahoo.com','2008-01-01','Faculty of Arts and Social Science','Language',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHOU XIALIN','zhouxialin1990@yahoo.com','2008-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('DAVID CHAPMAN','davidchapman1989@msn.com','2008-01-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('QIN YUWEI','qinyuwei2011@hotmail.com','2009-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('DENNIS PALMER','dennispalmer1992@yahoo.com','2010-01-01','Physical Science and Engineering','ME',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('DAVID HALL','davidhall1992@yahoo.com','2009-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('DING WEI XIANG','dingweixiang1990@yahoo.com','2010-01-01','health science','IS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('IRIS BROWN','irisbrown1992@hotmail.com','2008-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('KEN OWEN','kenowen2011@yahoo.com','2007-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('GE DUO','geduo2010@yahoo.com','2009-08-01','Physical Science and Engineering','ME',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('HUANG XUANTI','huangxuanti1992@msn.com','2007-08-01','health science','IS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('LISA SMITH','lisasmith2011@msn.com','2007-08-01','ICT','Chemistry',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('NG YONG MING','ngyongming2011@yahoo.com','2009-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHENG NANA','zhengnana1991@gmail.com','2010-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ZHAO YANG','zhaoyang1989@yahoo.com','2008-08-01','Faculty of Arts and Social Science','Geography',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('NG YAN FEN','ngyanfen2010@msn.com','2008-01-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('ANG JIA YI','angjiayi1990@hotmail.com','2009-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('CHNG HUI LING','chnghuiling1992@gmail.com','2009-01-01','Faculty of Arts and Social Science','History',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('NGOO KAI TING','ngookaiting1991@yahoo.com','2010-08-01','health science','CS',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('TSO HUI LIN','tsohuilin1989@msn.com','2007-08-01','ICT','Physics',null);
Insert into STUDENT (NAME,EMAIL,YEAR,FACULTY,DEPARTMENT,GRADUATE) values ('LIU ZHENCAI','liuzhencai1990@msn.com','2010-08-01','health science','CS',null);



Insert into BOOK (TITLE,FORMAT,PAGES,LANGUAGE,AUTHORS,PUBLISHER,YEAR,ISBN10,ISBN13) values ('The Digital Photography Book','paperback',219,'English','Scott Kelby','Peachpit Press','2006-01-01','032147404X','978-0321474049');
Insert into BOOK (TITLE,FORMAT,PAGES,LANGUAGE,AUTHORS,PUBLISHER,YEAR,ISBN10,ISBN13) values ('The Great Gatsby','hardcover',216,'English','F. Scott Fitzgerald','Scribner','1995-01-01','0684801523','978-0684801520');
Insert into BOOK (TITLE,FORMAT,PAGES,LANGUAGE,AUTHORS,PUBLISHER,YEAR,ISBN10,ISBN13) values ('Photoshop Elements 9: The Missing Manual','paperback',640,'English','Barbara Brundage','Pogue Press','2010-01-01','1449389678','978-1449389673');
Insert into BOOK (TITLE,FORMAT,PAGES,LANGUAGE,AUTHORS,PUBLISHER,YEAR,ISBN10,ISBN13) values ('Where Good Ideas Come From: The Natural History of Innovation','hardcover',336,'English','Steven Johnson','Riverhead Hardcover','2010-01-01','1594487715','978-1594487712');



Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('tsohuilin1989@msn.com','978-1449389673',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('gohhuiying1989@gmail.com','978-1449389673',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('wangna1990@yahoo.com','978-1449389673',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('jennyhunt1991@gmail.com','978-1449389673',1,'FALSE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('zhanghan1989@hotmail.com','978-1594487712',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('qinyuwei2011@hotmail.com','978-1594487712',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('zhuchang2010@gmail.com','978-1594487712',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('huangzhanpeng1992@msn.com','978-1594487712',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('liuzhencai1990@msn.com','978-0321474049',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('wangna1990@yahoo.com','978-0321474049',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('ngyongming2011@yahoo.com','978-0321474049',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('davidhall1992@yahoo.com','978-0321474049',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('liushaojun2010@msn.com','978-0684801520',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('tayweiguo1989@msn.com','978-0684801520',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('jennyhunt1991@gmail.com','978-0684801520',1,'TRUE');
Insert into COPY (OWNER,BOOK,COPY,AVAILABLE) values ('jennyhunt1991@gmail.com','978-0684801520',2,'TRUE');

Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhanghong2011@msn.com','tsohuilin1989@msn.com','978-1449389673',1,'2010-02-16','2010-02-20');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('kenowen2011@yahoo.com','tsohuilin1989@msn.com','978-1449389673',1,'2010-03-15','2010-06-06');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhaoyang1989@yahoo.com','tsohuilin1989@msn.com','978-1449389673',1,'2010-06-24','2010-07-23');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('geduo2010@yahoo.com','tsohuilin1989@msn.com','978-1449389673',1,'2010-06-22','2010-09-28');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('huangwenxin2010@msn.com','gohhuiying1989@gmail.com','978-1449389673',1,'2010-01-19','2010-02-05');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('pohhuiling1992@hotmail.com','gohhuiying1989@gmail.com','978-1449389673',1,'2010-04-09','2010-05-27');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('dingweixiang1990@yahoo.com','gohhuiying1989@gmail.com','978-1449389673',1,'2010-01-08','2010-01-26');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('tayweiguo1989@msn.com','gohhuiying1989@gmail.com','978-1449389673',1,'2010-06-11','2010-06-11');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhengnana1991@gmail.com','wangna1990@yahoo.com','978-1449389673',1,'2010-01-01','2010-03-24');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('davidchapman1989@msn.com','wangna1990@yahoo.com','978-1449389673',1,'2010-03-08','2010-04-23');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('irisbrown1992@hotmail.com','wangna1990@yahoo.com','978-1449389673',1,'2010-08-21','2010-09-12');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('huangzhanpeng1992@msn.com','wangna1990@yahoo.com','978-1449389673',1,'2010-01-11','2010-04-09');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhengxi1990@yahoo.com','jennyhunt1991@gmail.com','978-1449389673',1,'2010-01-17','2010-01-20');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('anniechapman1991@yahoo.com','jennyhunt1991@gmail.com','978-1449389673',1,'2010-05-17','2010-06-13');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhanghan1989@hotmail.com','jennyhunt1991@gmail.com','978-1449389673',1,'2010-06-25','2010-07-16');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('pohhuiling1992@hotmail.com','jennyhunt1991@gmail.com','978-1449389673',1,'2010-02-14','2010-02-15');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('dennispalmer1992@yahoo.com','zhanghan1989@hotmail.com','978-1594487712',1,'2010-01-26','2010-01-26');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhengzhemin1991@yahoo.com','zhanghan1989@hotmail.com','978-1594487712',1,'2010-05-13','2010-05-19');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('angjiayi1990@hotmail.com','zhanghan1989@hotmail.com','978-1594487712',1,'2010-03-25','2010-04-20');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('huangwenxin2010@msn.com','zhanghan1989@hotmail.com','978-1594487712',1,'2010-09-12','2010-10-05');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('tayyongming1989@gmail.com','qinyuwei2011@hotmail.com','978-1594487712',1,'2010-02-20','2010-03-01');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('ngqiyang1989@msn.com','qinyuwei2011@hotmail.com','978-1594487712',1,'2010-01-04','2010-01-16');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('siowcaokhoa1991@msn.com','qinyuwei2011@hotmail.com','978-1594487712',1,'2010-04-22','2010-05-07');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('pengjiayuan2011@hotmail.com','qinyuwei2011@hotmail.com','978-1594487712',1,'2010-03-07','2010-06-25');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('fengmeng1990@gmail.com','zhuchang2010@gmail.com','978-1594487712',1,'2010-02-25','2010-02-27');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('neelamdeol2011@hotmail.com','zhuchang2010@gmail.com','978-1594487712',1,'2010-03-05','2010-06-02');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('dennispalmer1992@yahoo.com','zhuchang2010@gmail.com','978-1594487712',1,'2010-01-02','2010-03-23');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('seahteckkee1990@gmail.com','zhuchang2010@gmail.com','978-1594487712',1,'2010-08-17','2010-11-07');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('ngyanfen2010@msn.com','huangzhanpeng1992@msn.com','978-1594487712',1,'2010-02-25','2010-02-27');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('ngookaiting1991@yahoo.com','huangzhanpeng1992@msn.com','978-1594487712',1,'2010-01-23','2010-03-15');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhuchang2010@gmail.com','huangzhanpeng1992@msn.com','978-1594487712',1,'2010-01-07','2010-01-23');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('liuzhanpeng2011@msn.com','huangzhanpeng1992@msn.com','978-1594487712',1,'2010-08-16','2010-08-25');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('nganhquang1991@yahoo.com','liuzhencai1990@msn.com','978-0321474049',1,'2010-02-23','2010-03-27');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('huangxuanti1992@msn.com','liuzhencai1990@msn.com','978-0321474049',1,'2010-02-27','2010-02-27');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('jennyhunt1991@gmail.com','liuzhencai1990@msn.com','978-0321474049',1,'2010-07-20','2010-08-09');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('dennisbeckham1989@msn.com','liuzhencai1990@msn.com','978-0321474049',1,'2010-02-06','2010-05-25');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhouxialin1990@yahoo.com','wangna1990@yahoo.com','978-0321474049',1,'2010-01-04','2010-03-20');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('choyyiting1992@hotmail.com','wangna1990@yahoo.com','978-0321474049',1,'2010-02-21','2010-03-06');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('liuyihui1990@hotmail.com','wangna1990@yahoo.com','978-0321474049',1,'2010-03-23','2010-03-27');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('davidchapman1989@msn.com','wangna1990@yahoo.com','978-0321474049',1,'2010-06-01','2010-08-01');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('jennyhunt1991@gmail.com','ngyongming2011@yahoo.com','978-0321474049',1,'2010-01-21','2010-02-09');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('choyyiting1992@hotmail.com','ngyongming2011@yahoo.com','978-0321474049',1,'2010-01-10','2010-03-08');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('geduo2010@yahoo.com','ngyongming2011@yahoo.com','978-0321474049',1,'2010-01-19','2010-01-20');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhuchang2010@gmail.com','ngyongming2011@yahoo.com','978-0321474049',1,'2010-01-14','2010-04-22');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('dennispalmer1992@yahoo.com','davidhall1992@yahoo.com','978-0321474049',1,'2010-02-11','2010-02-26');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('lisasmith2011@msn.com','davidhall1992@yahoo.com','978-0321474049',1,'2010-05-03','2010-06-10');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('chnghuiling1992@gmail.com','davidhall1992@yahoo.com','978-0321474049',1,'2010-03-23','2010-05-16');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('liuzhencai1990@msn.com','davidhall1992@yahoo.com','978-0321474049',1,'2010-08-19','2010-11-18');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('qinyiyang2010@hotmail.com','liushaojun2010@msn.com','978-0684801520',1,'2010-01-17','2010-01-20');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('zhouxialin1990@yahoo.com','liushaojun2010@msn.com','978-0684801520',1,'2010-07-17','2010-08-13');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('davidhall1992@yahoo.com','liushaojun2010@msn.com','978-0684801520',2,'2010-05-08','2010-05-26');                                                                                                 
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('davidhall1992@yahoo.com','jennyhunt1991@gmail.com','978-0684801520',2,'2010-05-08','2010-05-26');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('anniechapman1991@yahoo.com','tsohuilin1989@msn.com','978-1449389673',2,'2010-07-17','2010-08-13');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('anniechapman1991@yahoo.com','jennyhunt1991@gmail.com','978-0684801520',1,'2010-07-17','2010-08-13');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('anniechapman1991@yahoo.com','tsohuilin1989@msn.com','978-1449389673',1,'2011-07-17','2011-08-13');
Insert into LOAN (BORROWER,OWNER,BOOK,COPY,BORROWED,RETURNED) values ('anniechapman1991@yahoo.com','jennyhunt1991@gmail.com','978-0684801520',1,'2011-07-17','2011-08-13');

