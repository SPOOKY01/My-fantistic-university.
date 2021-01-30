#GovRptWordCloudV1.py
import jieba
import wordcloud
f = open("关于实施乡村振兴战略的意见.txt","r",encoding="utf-8")
t = f.read()
f.close()
ls = jieba.lcut(t)
txt = " ".join(ls)
w = wordcloud.WordCloud(font_path = "msyh.ttc",width = 1600, height = 900,background_color = "white",)
w.generate(txt)
w.to_file("BIG.png")
