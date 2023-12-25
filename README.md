# 开发环境
windows C++  
依赖：Json Opencv(C++)  


# 命令解析
-i: 的参数表示输入文件的类型
   - 1 ：  binarySARData
   - 2 ：  jsonSARData
   - 3 ：  otherSARData

-o: 的参数表示输出文件的类型
   - 1 ：  binarySARData
   - 2 ：  jsonSARData
   - 3 ：  otherSARData

-m: 的参数表示要对图像执行的操作
   - 1 ：  meanAmplitude
   - 2 ：  amplitudeNormalize

-f: 的参数表示存放图像路径的text文件的路径


example：test.exe -i 1 -o 2 -m 1 -f image_path.txt  
表示：输入文件类型为binarySARData，输出文件类型为jsonSARData，执行meanAmplitude操作，输入图像的路径保存在可执行文件test.exe相同的目录下的image_path.txt 中

# 单元测试

测试点：
  - 是否可以正确解析main函数的参数  
  - 是否可以将结果输出为二进制文件  
  - 是否可以将结果输出为Json文件  
  - 是否可以生成jpeg可视图像   
  - 生成平均幅度图的结果值是否正确
  - 生成归一化标准差的值是否正确
  - SARData类是否可以正确返回数据

测试方法：通过打印对应位置，相关参数的值，判断上面的功能点是否正常
