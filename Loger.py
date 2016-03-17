#!/usr/bin/env python
#-*- coding:utf-8 -*-  
#-- Author: Crh
#----------------
import socket  
import sys
import thread
import Tkinter
import os
import platform
import tkMessageBox
import tkFileDialog 
import subprocess
import json
from Tkinter import *

HOST = '127.0.0.1'
PORT = 6678

def initSocket( threadName, time, uip):
  while 1:#连接循环
    try:
      s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
      s.connect((HOST,PORT))
    except:
      #不断尝试连接
      # print "连接中"
      continue
    # print "连接完成"
    uip.addSocketData( "-----------连接完成，新开启游戏-----------\n" )
    # dd = "debugmsg on"
    # s.sendall("debugmsg on")
    while 1:#收信息循环
        data=s.recv(1024)
        if data != "":
          # print data         #输出变量
          uip.addSocketData( data )
        if not data:
          #scoket断开退出接收信息循环进入连接循环
          break
    s.close()   #关闭连接 


class LogerUI(object):
  # 私有变量
  __root = None # tk
  __rollVar = None # 滚动log选项
  __pjVar = None # 工程路径
  __slVar = None # 模拟器路径
  __logText = None # 显示log控件
  __showList = None # listbox控件 分辨率
  __config = None # 默认信息保存
  __window = None # 模拟器进程
  __screen = None # 分辨率套餐
  # 构造函数
  def __init__( self ):
    pass

  # 析构函数
  def __del__( self ):
    pass

  # 初始化界面
  def initUI( self ):
    self.readJson()
    self.__root = Tk()
    self.__root.title("Loger")
    self.__root.geometry('640x480')
    self.__root.columnconfigure(0, weight=1)  
    self.__root.rowconfigure(0, weight=1) 

    self.__rollVar = IntVar()
    self.__pjVar = StringVar()
    self.__pjVar.set( self.getJsonByKey("ProjectPath") )
    self.__slVar = StringVar()
    self.__slVar.set( self.getJsonByKey("SimulatorPath") )

    scrollbar = Scrollbar(self.__root)
    self.__logText = Text(self.__root, width=88, height=20)
    self.__logText.bind( "<KeyPress>",lambda e:"break" )

    self.__showList = Listbox(self.__root)
    
    for i in range(len(self.__screen)):
      self.__showList.insert(i, self.__screen[i])

    self.__showList.grid(column=2, row=2, sticky='nwe')
    self.__showList.selection_set( self.getJsonByKey("PowerTag") )

    ePJ = Entry(self.__root, bd =5, textvariable=self.__pjVar)
    ePJ.grid(column=0, row=0, sticky='we')

    btnPJ = Tkinter.Button(self.__root, text ="项目工程路径", command = self.addProjectPath)
    btnPJ.grid(column=1, row=0, sticky='we')
    
    eSL = Entry(self.__root, bd =5, textvariable=self.__slVar)
    eSL.grid(column=0, row=1, sticky='we')

    btnSL = Tkinter.Button(self.__root, text ="模拟器路径", command = self.addSimulatorPath)
    btnSL.grid(column=1, row=1, sticky='we')


    self.__logText.grid(column=0, row=2, sticky='nswe')
    scrollbar.grid(column=1, row=2, sticky='nsw')
    self.__logText.configure(yscrollcommand=scrollbar.set)
    scrollbar.configure(command=self.__logText.yview)

    btnOpen = Tkinter.Button(self.__root, text ="运行", command = self.openCallBack)
    btnOpen.grid(column=2, row=2, sticky='swe')

    btnDel = Tkinter.Button(self.__root, text ="清除Log", command = self.delCallBack)
    btnDel.grid(column=2, row=3, sticky='swe')

    cbRun = Checkbutton(self.__root, variable = self.__rollVar, text = '自动滚动', onvalue = 0, offvalue = 1, command = self.checkCallBack)
    cbRun.grid(column=0, row=3)

  def runUI( self ):
    self.__root.mainloop()

  # 回调函数
  # 添加项目路径
  def addProjectPath( self ):
    filename = tkFileDialog.askdirectory(parent=self.__root,initialdir="/",title='project path')
    self.__pjVar.set( filename )
    self.writeJson( "ProjectPath", filename )

  # 添加模拟器路径
  def addSimulatorPath( self ):
    filename = ""
    if platform.system() == "Windows":
      filename = tkFileDialog.askopenfilename(parent=self.__root,initialdir="/",defaultextension=".exe",title='Pick a exe')
    else:
      filename = tkFileDialog.askopenfilename(parent=self.__root,initialdir="/",defaultextension=".app",title='Pick a app')
    self.__slVar.set( filename )
    self.writeJson( "SimulatorPath", filename )
    

  # 运行
  def openCallBack( self ):
    command = self.__slVar.get()
    if not os.path.exists( command ):
      tkMessageBox.showinfo( "路径错误", "没有输入模拟器路径或者路径错误")
      return

    if len(self.__showList.curselection()) <= 0:
      tkMessageBox.showinfo( "", "请选择模拟器的分辨率")
      return

    if platform.system() != "Windows":
      command = self.checkFile( command )
    if self.__window != None:
      subprocess.Popen.terminate(self.__window)
      self.__window = None
    self.writeJson( "PowerTag", self.__showList.curselection()[0] )
    command = command + " -p " + self.__pjVar.get() + " -s " + self.__showList.get(self.__showList.curselection()) + " "
    self.__window = subprocess.Popen( command, shell=True )

  # 删除log
  def delCallBack(self):
    self.__logText.delete(0.0, Tkinter.END)

  # 滚动选项
  def checkCallBack(self):
    pass

  # 处理函数
  def addSocketData( self, data ):
    if self.__logText == None:
      print " ------------- logText None ------------- "
      return
    self.__logText.insert(END, data)
    if self.__rollVar.get() == 0:
      self.__logText.see(END)

  def delDataString( self, data ):
    temp = 0
    pos = data.find( ".app", 0 )
    pos = len(data) - pos
    data = data[:-pos]
    for i in range(len(data) - 1, 0, -1):
      if data[i] == "/":
        return data[:-(len(data) - i)]
    return data


  def getFileDir( self ):
    #获取脚本路径
    fpath = repr(os.path.dirname(os.path.realpath(sys.argv[0])))
    fpath = fpath[:-1]
    fpath = fpath[1:]
    if platform.system() != "Windows":
      fpath = self.delDataString( fpath )
    return fpath

  def checkFile( self, path ):
    pp = path + "/Contents/MacOS/"
    names = os.listdir(pp)
    for n in names:
      return pp+n

  # 读取配置json 初始化记录
  def readJson( self ):
    filepath = self.getFileDir() + "/logerconfig.json"
    if os.path.exists( filepath ):
      fp = open( filepath, 'r' )
      self.__config = json.loads( fp.read() )
      self.__screen = self.__config[ 'Screen' ]
      fp.close()
    else:
      self.__config = { 'ProjectPath':" ", 'SimulatorPath':"", 'PowerTag':0 }
      self.__screen = [ "960*640", "1136*640", "1334*750", "1024*768", "1920*1080", "1280*800", "640*960", "640*1136", "768*1024", "750*1334", "1080*1920", "800*1280"]
      self.__config[ 'Screen' ] = self.__screen
      self.writeJson( "all", self.__config )

  # 把信息写入json保存
  def writeJson( self, key, data ):
    if key != "all":
      self.__config[ key ] = data
    filepath = self.getFileDir() + "/logerconfig.json"
    fp = open( filepath, 'w' )
    fp.write(json.dumps(self.__config, ensure_ascii=False))
    fp.close()

  # 获取配置对应key的信息
  def getJsonByKey( self, key ):
    return self.__config[ key ]
      

# 主函数 -------------------------------------
if __name__ == '__main__': 
  # 初始化ui对象
  ui = LogerUI()
  ui.initUI()  
  # 开启线程 socket
  thread.start_new_thread( initSocket, ("Thread-Socket", 2, ui) )  
  # 主线程是UI, 初始化界面 
  ui.runUI()
    

    
                