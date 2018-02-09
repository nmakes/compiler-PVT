with open("CFG.txt") as f:
    content = f.readlines()

content = [x.strip() for x in content] 
while '' in content: content.remove('')
tags=[]
for ele in content:
	aa=ele.split("===>")[0]
	aa=aa.strip()
	if aa in tags: 
		continue
	else :
		tags.append(aa)


terminals=[]
for ele in content:
	lst=(ele.strip()).split(" ")
	for elem in lst: 
		elem= elem.strip()
		if elem not in terminals and elem not in tags: terminals.append(elem)


terminals.remove("===>")
print "TERMINALS"
print terminals
print
print "NON TERMINALS"
print tags
# print len(tags)
# print content
