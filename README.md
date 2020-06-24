本系统由QT 5.14.1进行开发 
系统需要安装python和对opencv进行QT编译  编译流程如下https://blog.csdn.net/hit1524468/article/details/79227636
编译完的Opencv放在E:\opencvProject\opencvForQt 目录下 或者自行修改原代码的pro文件INCLUDEPATH和LIBS

文件中发行版可以直接运行 
本系统有一位默认管理员 账号0 密码0 
系统有三位普通用户 帐号密码相同分别是1 2 3   
新注册员工账号为工号 密码为000000 

使用过程：
1.首先进行新员工注册和人脸采集 填写完信息后打开摄像头按采集按钮采集人脸 采集完成点击生成训练数据→选择训练文件→开始训练
2.训练完成看到./src目录下有三个XML文：EnterpriseEigenFaceModel/EnterpriseFaceFisherModel/EnterpriseLBPHModel件即可
3.使用普通用户登录系统 点击开始签到（如果提示不可签到 请使用管理员登录到签到设置那里进行更改） 正脸对准摄像头即可签到 签到成功有提示 点击查询可以查看当天个人签到信息
4.管理员可以在菜单那里查看全体员工考勤
