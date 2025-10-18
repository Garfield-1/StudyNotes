--设置.so搜寻路径,若动态库与lua文件在同一目录下则无需指定
package.cpath = "./?.so" 

local myLib = require "mylib"
local add

myLib.printDemo()

add = myLib.addDemo(1, 12)
print(add)