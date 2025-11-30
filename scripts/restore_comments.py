import io
import os
import subprocess

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FILES = [
    "Classes/Animals/Animal.cpp",
    "Classes/Animals/Animal.h",
    "Classes/App/AppDelegate.cpp",
    "Classes/Inventory/Chest.cpp",
    "Classes/Inventory/Chest.h",
    "Classes/App/Constant.h",
    "Classes/Scenes/CreateErrorScene.cpp",
    "Classes/Farming/Crop.cpp",
    "Classes/Scenes/GameScene.cpp",
    "Classes/Scenes/GameScene.h",
    "Classes/Scenes/LoadingScene.cpp",
    "Classes/Scenes/MenuScene.cpp",
    "Classes/Scenes/NewSelectScene.cpp",
    "Classes/Characters/Player.cpp",
    "Classes/Characters/Player.h",
    "Classes/Scenes/SaveSelectScene.cpp",
    "Classes/Systems/UIManager.cpp",
    "Classes/Systems/UIManager.h",
    "Classes/Inventory/item.cpp",
    "Classes/Inventory/item.h",
    "Classes/Inventory/itemManager.cpp",
    "Classes/Inventory/itemManager.h",
    "Classes/World/map.cpp",
    "Classes/Characters/npcTemplate.h",
    "Classes/Systems/system.cpp",
    "Classes/Inventory/tool.cpp",
    "Classes/Inventory/toolManager.cpp",
    "Classes/Inventory/toolManager.h",
]


def git_show(path):
    target = "origin/main:%s" % path.replace("\\", "/")
    proc = subprocess.Popen(
        ["git", "show", target],
        cwd=REPO_ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = proc.communicate()
    if proc.returncode != 0:
        raise RuntimeError("git show failed for %s: %s" % (target, stderr))
    try:
        return stdout.decode("utf-8")
    except UnicodeDecodeError:
        return stdout.decode("gbk", "ignore")


def read_file(path):
    with io.open(path, "r", encoding="utf-8", errors="ignore") as fh:
        return fh.read()


def write_file(path, text):
    with io.open(path, "w", encoding="utf-8", newline="") as fh:
        fh.write(text)


def find_include_block(lines):
    start = None
    for idx, line in enumerate(lines):
        if line.lstrip().startswith("#include"):
            start = idx
            break
    if start is None:
        return None
    end = start
    total = len(lines)
    while end < total:
        stripped = lines[end].strip()
        if stripped.startswith("#include") or stripped == "":
            end += 1
            continue
        break
    return start, end


def merge_content(remote_text, local_text):
    remote_lines = remote_text.splitlines()
    local_lines = local_text.splitlines()

    remote_block = find_include_block(remote_lines)
    local_block = find_include_block(local_lines)

    if not remote_block or not local_block:
        return remote_text

    r_start, r_end = remote_block
    l_start, l_end = local_block

    merged_lines = list(remote_lines[:r_start])
    merged_lines.extend(local_lines[l_start:l_end])
    merged_lines.extend(remote_lines[r_end:])

    return "\n".join(merged_lines) + "\n"


def main():
    updated = []
    for rel_path in FILES:
        local_path = os.path.join(REPO_ROOT, rel_path)
        if not os.path.exists(local_path):
            print("[skip] %s not found" % rel_path)
            continue
        remote_rel = "Classes/%s" % os.path.basename(rel_path)
        remote_text = git_show(remote_rel)
        local_text = read_file(local_path)
        merged_text = merge_content(remote_text, local_text)
        if merged_text != local_text:
            write_file(local_path, merged_text)
            updated.append(rel_path)
            print("[update] %s" % rel_path)
        else:
            print("[keep] %s" % rel_path)
    print("Updated %d files" % len(updated))


if __name__ == "__main__":
    main()
