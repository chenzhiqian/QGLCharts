# QGLCharts
基于OpenGL开发的二维图表组件，包含折线图、柱状图、散点图以及组合图。

# 基础说明
应公司大数据量绘图需要，基于OpenGL定制的二维图表组件，供相互学习。
当前使用opengl老接口进行绘制(glBegin/glVertex3f/glEnd)，后续有需要修改成GSLS。

# 工程类图
![微信截图_20210312192140](https://user-images.githubusercontent.com/9159331/110934722-5e30e700-8369-11eb-891b-da76e555792b.png)


# 功能说明
1. 支持图表样式设置: 设置坐标系网格、刻度、颜色、文本位置、背景色等。
2. 鼠标位置滚轮缩放，放大后鼠标拖拽滚动。目前暂只支持x轴缩放，y轴不缩放，
3. 表格内区域分块选中高亮
4. 数据区域高亮
5. 支持分页显示，分页后支持鼠标拖拽滚动。

# 效果图
![image](https://user-images.githubusercontent.com/9159331/110934842-8c162b80-8369-11eb-8f50-71bb7057c988.png)
![image](https://user-images.githubusercontent.com/9159331/110934860-933d3980-8369-11eb-8701-07db19cae765.png)
![image](https://user-images.githubusercontent.com/9159331/110934872-9afcde00-8369-11eb-9507-ff5cb88fd771.png)
![image](https://user-images.githubusercontent.com/9159331/110934896-a3edaf80-8369-11eb-94fa-adbb1fd7a4b9.png)

# 接口说明
相关接口及属性配置，请查看代码注释
