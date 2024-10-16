from os import path
path = path.dirname(__file__)+"/"

ft = open(path+"cheatText.txt", "r").read()

def parse():
  f = open(path + "src/displayapp/screens/Cheat_text.h", "w")
  f.write('#include "displayapp/screens/Cheat.h"\n#pragma once\n')

  topics = {}
  curr_topic = ""
  for line in ft.splitlines():
    if line.endswith(":"):
      curr_topic = line[:-1]
      topics[curr_topic] = {
        "lines": [],
        "title": curr_topic,
        "macro": curr_topic.upper().replace(" ", "_")
      }
    elif line.startswith("  "):
      topics[curr_topic]["lines"].append(line[2:])
  for topic in topics:
    f.write("#define " + topics[topic]["macro"] + " " + '"' + '\\n'.join(topics[topic]["lines"]) + '"\n')
  f.write("static Topic *topics;\nconst unsigned pagesNum = " + str(len(topics)) + ";\n")
  f.write("Topic *setup_topics() {\n")
  f.write("\ttopics = new Topic[2] {\n")
  for topic in topics:
    f.write("\t\t{\"" + topics[topic]["title"] + "\", " + topics[topic]["macro"] + "},\n")
  f.write("\t};\n")
  f.write("\treturn topics;\n")
  f.write("}")
  f.close()

if __name__ == "__main__":
  parse()