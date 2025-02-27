## 项目介绍
- 此工程项目作为大二下数据结构课程的大作业，可以实现图书馆管理以及读者交互的功能。
- 后端采用C++编写，图形化采用qt编写。
- 未实现数据库和网络功能，数据和日志均写在文件中，可实现单机运行测试

## 版权及免责声明
1、本软件的著作权归北京邮电大学2015级计算机学院三位大学生所有，请您不要私自改动传播。  
2、本软件仅供个人学习参考，不得用于商业或者其他用途，否则，一切后果均由您自己承担，我们对此不承担任何法律责任。   

## 使用方法
打开./library_exe/bin/Library.exe

1、	登录。分为读者登录和管理员登录，读者登录名为学/工号，默认密码为学/工号，可以自行修改。默认管理员账号admin，密码weiwuzhizun，管理员账户也可以在管理员权限下自行增删或修改密码。以下2-15为管理员功能，17-18为读者功能，16更改账号为通用功能。

2、	主界面从上至下分别是标题栏、菜单栏、选项卡界面和状态栏。选项卡界面包含了主要功能的按钮，菜单栏则包含了全部功能及其快捷方式。

3、	各大管理界面（包括图书管理界面、读者管理界面、借阅/预约管理界面、出版社管理界面、学院/部门管理界面、权限/读者类型管理界面、图书分类管理界面等）的相同功能按钮，我们一并说明。  
新建：添加一条记录，例如在读者管理界面添加一名读者，在图书管理界面添加一本图书等。  
删除：删除一条记录，例如在出版社管理界面删除一个出版社，在学院/部门管理界面  
选择：只有在特定情况下，此按钮才会变为可点击状态。例如，在新建一条借阅信息时，管理员选择读者，此时显示出来的读者界面的选择按钮即为可点击状态，点击后即可选中该读者。  
修改：修改一条记录，权限/读者类型管理界面修改一个权限的具体信息等。  
搜索：顶部的输入框、搜索按钮以及关键字选择框共同组成了搜索功能。例如在图书管理界面中，关键字选择书名，在输入框内输入书名，再点击搜索，即可显示相关条目。  
导出：导出当前显示的条目信息到excel中  
显示全部：点击可以无视关键字，显示全部信息  
排序：点击表单头可以对对应列进行升/降序显示  

4、	借阅/预约管理的相关按钮  
借阅/预约：跳出新建界面，可以新建一条借阅/预约信息，如果所借图书没有可借或可预约册，则加入该书的等待队列。  
归还/取消预约：归还选中图书、取消预约选中图书。  
预约改借阅：将书籍的预约状态改为借阅状态  
查看：查看借阅/预约信息  

5、	日期模拟  
变速：设置模拟速率，单位为秒/天  
开始模拟：进入模拟模式，若没有鼠标键盘操作，则自动计时，根据设定的速率更新日期。若有鼠标键盘操作，则不会更新日期。  
停止模拟：离开模拟模式  
日期重置：只是为了方便演示设置的功能，可以重置系统日期，日期可选，但将删除所有借阅、预约、日志、统计等信息。  

6、	日志  
所有的日志文件保存在logs文件夹中，包括每天的操作记录、图书的历史借阅/预约信息、读者的历史借阅/预约信息等。

7、	统计  
包括三种统计图：七天借阅率、被借阅书籍前十、读者借书前十；另外包括每本书的借阅率，定义为该书所有册的外借天数/该书所有册的在馆天数（从添加进图书馆到现在的累计天数）。每本书的借阅率显示在图书管理界面的表单中。

8、	读者扣费机制和充值  
扣费机制：每册书每超期一天，扣费金额为该书的价格*0.01。点击菜单栏的新建，找到余额充值，选择读者后即可充值相应金额。若读者余额低于0元，将无法继续借阅或者预约。

9、	图书管理  
管理图书信息，每本书包括书名、作者、ISBN、出版社、分类号、种次号、价格、简介等信息。其中出版社和分类号可以调出相应界面进行很方便地选择。种次号为图书在该类图书中的添加顺序，自动生成。由于同一本书可以有好几册，所以我们为每本书增加了册管理界面，可以新建、删除、修改册信息，册信息包括出版日期（每册书出版日期可能不一致）、册序号（用于区分同一本书的不同册）等。新建一本图书时，至少要添加一册书。每册书另外还包括添加信息、是否被借阅/预约、借阅/预约者信息、索书号等信息，其中索书号由分类号和种次号和册序号组成后自动生成。

10、	读者管理  
管理读者信息，每名读者包括姓名、学/工号、性别、权限/读者类型、逾期天数、账户余额、所在院系/部门等信息，在修改信息界面还可以看到该读者当前的借阅和预约信息。
逾期天数为每册书每超期一天增加一。

11、	出版社管理  
管理出版社信息，增删修改出版社信息，每个出版社包括出版社名、ISBN编号、所在地等信息。

12、	学院/部门管理  
管理学院/部门信息，增删修改学院/部门信息，用于添加修改读者时在选择框内选择学院/部门信息。

13、	权限/读者类型管理  
包括读者类型、权限编号、借阅/预约册数上限、借阅天数上限、续借次数上限等信息，可以增删修改权限，用于添加修改读者时在选择框内选择权限/读者类型，设定该读者的权限。

14、	图书分类管理  
包括分类号、分类名等信息，可以增删分类，以树型结构显示，方便管理员在添加修改图书时选择分类。

15、	管理员账户管理  
可以增删修改管理员账户信息，可以修改密码。

16、	更改账号  
可以退回到登录界面，重新登录管理员账户或读者账户。

17、	读者界面  
可以查看读者的个人信息，修改密码，查看当前的借阅、预约记录，可以搜索图书信息，可以对所借图书进行续借，可以预约图书等。

18、	通知界面  
在余额不足、所借书籍超期、自动预约图书的情况下读者会收到通知信息。信息如果处于未读状态，字体加粗。最新通知显示在通知表单的最前面，标有日期信息。

## 目录结构
- dat/ 图书等数据
- images/ 图片
- library/ 源代码
- Logs/ 日志
- option/ 配置
- statistic/ 统计数据
- library_exe/bin/Library.exe 免安装可运行程序

## 项目开发环境配置
- 官网下载安装Qt5.12.2，需安装Qt charts等模块，建议全部安装
- IDE：Qt Creator
- 编译器: MinGW32，建议安装使用Qt自带的
- Qt的项目配置文件中配置`QT       += core gui sql printsupport`

## 功能需求说明
本次设计旨在实现模拟图书馆管理系统，赋予每个教师，学生不同的借书权限和借书数量，建立图书馆管理系统，实现包括图书的购入、编码、借出、还回、统计、查询等功能，形成一个能在Windows系统上运行的程序，使得用户能直观理解操作本软件。为了设计此软件，我们针对现有的学校图书馆进行调研，从中抽象出我们所需要的数据结构。其中每种图书包括图书编号、书名、作者、出版社、简介、购入时间、数量等信息。教师、学生包括姓名、工作证（学生证）号、所在学院、借书权限、借书数量等信息。此外还设计了出版社设计了名字，所属城市，ISBN编号。

### 账户注册登录及分类
账户主要分为两类：管理员及读者。这两类有着不同的权限。读者可以进行账号信息的查询，借阅或预约图书信息的查询，线上书籍续借或预约、查看通知等。管理员具有一切新建、修改、删除权限。

### 图书借阅，预约及归还功能:
当用户点击图书借阅时，会显示当前书籍的信息，书名、作者、出版社、ISBN、共几册、是否被借阅、预约人数等等，读者可以根据这些信息判断是否要进行借阅或者预约。在归还时删除相应的借阅信息即可。

### 读者信息录入、修改、注销、管理员账户管理
管理员可以对读者的信息进行修改、注销。同时，每录入一份读者信息，就会生成一份读者账户，登录名为读者名字，密码为学号。管理员账号登陆下可以注册另一个管理员账号，同时可以实现对已有账号的修改和注销。所有账号的密码保存时均经过加密。

### 新建图书及图书管理
管理员账号登录下可以进行图书的录入，出版社信息可以根据已有的表单进行检索和匹配，书籍类别也可以根据已有的表单进行匹配，同时也可以进行已有种类图书册数的添加。管理图书包括图书信息的修改以及删除书籍。

### 借阅预约时间管理、读者余额管理
每一个读者类型都有一个借阅书籍时间上限，当超过这个上限，系统会生成一份通知送达读者，提醒读者借阅图书已超期且已扣费，同时管理员可以对读者进行余额充值的操作。

### 其他管理模块
主要包括：出版社管理、书籍类别管理、读者权限管理、学院管理等。出版社可以自行添加、修改和删除，书籍类别可以自行修改或删除，可以新建读者权限、并实现修改和删除，学院管理也可以添加修改删除学院。

### 时间控制及日志模块
当选择了快进模式时，可以实现当没有鼠标操作即以给定的速率加快运行时间，同时当有上述操作时，相应的记录会填写到日志文件中。

### 统计模块
进行相应数据的随着时间变化的统计，并以数据及柱状图或者折线图的方式展现出来，给使用者直观的感受，并在文件中有相应的记载，便于管理者使用。

## 功能模块
![](doc_images/功能模块图.png)

## 界面效果展示
### 登录界面
- 登录界面
    - ![](doc_images/登录界面.png)
- 登录失败
    - ![](doc_images/登录失败.png)
### 管理员界面
- 图书管理
    - 图书管理菜单
        - ![](doc_images/图书管理菜单.png)
    - 图书列表
        - ![](doc_images/图书列表.png)
    - 添加/修改图书
        - ![](doc_images/添加图书.png)
    - 至少添加一册书
        - ![](doc_images/至少添加一册书.png)
- 读者管理
    - 读者管理菜单
        - ![](doc_images/读者管理菜单.png)
    - 读者列表
        - ![](doc_images/读者列表.png)
    - 添加/修改读者
        - ![](doc_images/添加修改读者.png)
    - 读者余额充值
        - ![](doc_images/读者余额充值.png)
    - 读者余额不足
        - ![](doc_images/读者余额不足.png)
- 图书借阅/预约管理
    - 图书借阅预约管理菜单
        - ![](doc_images/图书借阅预约管理菜单.png)
    - 归还/续借
        - ![](doc_images/归还续借.png)
    - 取消预约/预约转借阅
        - ![](doc_images/预约.png)
    - 添加借阅/预约
        - ![](doc_images/添加借阅预约.png)
    - 查看借阅预约信息
        - ![](doc_images/查看借阅预约信息.png)
    - 预约等待队列
        - ![](doc_images/预约等待队列1.png)
        - ![](doc_images/预约等待队列2.png)
    - 续借超过次数
        - ![](doc_images/续借超过次数.png)
    - 借阅预约数量达到上限
        - ![](doc_images/借阅预约数量达到上限.png)
- 读者权限管理
    - ![](doc_images/读者权限管理.png)
- 图书分类管理
    - ![](doc_images/图书分类管理.png)
- 出版社管理
    - ![](doc_images/出版社管理.png)
- 学院部门管理
    - ![](doc_images/学院部门管理.png)
- 管理员账户管理
    - ![](doc_images/管理员账户管理.png)
- 统计信息
    - 统计信息菜单
        - ![](doc_images/统计信息菜单.png)
    - 七天借阅率
        - ![](doc_images/借阅率.png)
    - 被借阅图书前十
        - ![](doc_images/借阅书籍前十.png)
    - 读者借书前十
        - ![](doc_images/借阅读者前十.png)
- 日期模拟
    - 日期模拟菜单
        - ![](doc_images/日期模拟菜单.png)
    - 调整模拟速率
        - ![](doc_images/调整模拟速率.png)
    - 日期重置
        - ![](doc_images/日期重置.png)

### 读者界面
- 读者主界面
    - ![](doc_images/读者主界面.png)
- 读者个人信息
    - ![](doc_images/读者个人信息.png)
- 读者通知
    - ![](doc_images/读者通知.png)
- 图书搜索界面
    - ![](doc_images/图书搜索界面.png)
- 读者查看图书信息
    - ![](doc_images/读者查看图书信息.png)
