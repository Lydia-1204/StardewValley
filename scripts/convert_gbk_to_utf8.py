import io
import os

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


def convert_file(rel_path):
    path = os.path.join(REPO_ROOT, rel_path)
    if not os.path.exists(path):
        print("[skip] %s not found" % rel_path)
        return False
    with open(path, "rb") as fh:
        data = fh.read()
    try:
        text = data.decode("gbk")
    except UnicodeDecodeError:
        print("[warn] %s cannot be decoded as GBK" % rel_path)
        return False
    with io.open(path, "w", encoding="utf-8", newline="") as fh:
        fh.write(text)
    print("[convert] %s" % rel_path)
    return True


def main():
    converted = 0
    for rel_path in FILES:
        if convert_file(rel_path):
            converted += 1
    print("Converted %d files" % converted)


if __name__ == "__main__":
    main()
