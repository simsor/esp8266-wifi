Import("env")

import shutil, os
from htmlify import html_minify, js_minify, css_minify


def convert(f, dest):
    if os.path.isdir(f):
        for f2 in os.listdir(f):
            convert(f2)
    else:
        if f.endswith(".html"):
            content = open(f, "r").read()
            html_minify(content, comments=False)
            open(dest, "w").write(content)


def before_spiffs(source, target, env):
    project_dir = env["PROJECT_DIR"]
    data_dir = env["PROJECTDATA_DIR"]
    data_plain_dir = os.path.join(project_dir, "data_plain")
    if not os.path.exists(data_dir):
        os.mkdir(data_dir)

    for f in os.listdir(data_plain_dir):
        convert(f, os.path.join())


env.AddPreAction("$BUILD_DIR/spiffs.bin", before_spiffs)