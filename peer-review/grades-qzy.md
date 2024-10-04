> 满分应该是20分吧



## p1_1

总分：13.5



扣分点（英文部分摘自姥姥的评分细则，适当减少某些扣分）：

- the introduction is a simple copy + paste of the assignment statement (-0.3)
- messy – some of the data in the charts and tables are missing (-0.2) 
- the hand-in is not zipped with proper folders (-0.5)
- there is no comments on the testing results (-1)
- **analysis** of the **complexities** of time and/or space is missing -- they must show how they have reached their conclusions, instead of simply listing them (-2)
- 程序的输出结果太不友好，显示的是一堆数字，就问你们自己想用自己写出来的搜索引擎吗(-0.5)
- 缺少注释：hash.h、insert_index.cpp、main.cpp（还出现了中文注释）、search_word.cpp、string_view.h（看起来是你们引用的，但好像没有在报告中说明，存疑，这个就不扣了）(-2)

其他瑕疵，但不扣分：

- 既然已经设计了Linux的编译，那你们就没有想到过Linux不能正常运行.exe文件吗
- README 写的也不那么friendly

评价：

说实话，这是我看到过的最敷衍的Project，我之前在FDS上也没有遇到过这种情况，因为无论是从性能上，还是从美观上，尽管没有什么亮点，但也有不少的缺点（~~该不会是 1 + 1 < 2 吧~~）。

- 整个Project给我的感受是像是两三天内赶出来的样子，不知你们是不是这种情况
- 基本功能是完成的了，但看起来你们仅能做到这一点，甚至连测试和分析都没好好写（可能是来不及写了吧）
- 报告看起来是用了某人的模板，但我记得如果正常用他的模板的话，应该不会有这么丑吧

建议：

- 多腾出点时间，多付出点汗水，多走一点心，我相信这篇报告远远不是你们的上限，继续打磨吧！
- 对自己提出更严格的要求，不要让报告看起来如此松散敷衍



## p1_2

总分：20（-1 + 2 = 没扣）



扣分点：

- 缺少空间复杂度分析(-1)

瑕疵（不扣分）：

- 日期写错啦~
- 关于README，感觉有些内容最好明确地说出来
	- Clone the repository and navigate to the **root project directory** 指的是 `code` 目录吧
	- 测试部分运行 `ctest` 命令应该是在 `code/build` 目录下吧
- Declaration 最好用第一人称复数吧，感觉就像只有一个人做了保证…

评价：

1. 近乎完美的作品！不仅实现了基本的搜索引擎功能，而且也通过 **Disk Index Merging** 节省内存空间来完成 Bonus 目标
2. 整个文件结构分划合理规范，方便评分者的运行与测试这些程序
3. 报告写的相当出色，集美观、简洁、流畅与一身，而且分析部分十分有条理，尤其是伪代码那块部分值得我们学习
4. 最后向大佬献上我的膝盖 orz



