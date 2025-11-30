import difflib
import io
import os
import subprocess
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CLASSES_DIR = os.path.join(REPO_ROOT, "Classes")
LOG_PATH = os.path.join(os.path.dirname(__file__), "verify_classes_diff.log")


def _to_unicode(value):
    try:
        unicode_type = unicode  # pylint: disable=undefined-variable
    except NameError:  # pragma: no cover
        unicode_type = str

    if isinstance(value, unicode_type):
        return value
    if isinstance(value, str):
        try:
            return value.decode("utf-8")
        except Exception:  # pylint: disable=broad-except
            return value.decode("utf-8", "ignore")
    try:
        return unicode_type(str(value), "utf-8")
    except Exception:  # pylint: disable=broad-except
        return unicode_type(str(value), "utf-8", "ignore")


def safe_print(text):
    data = _to_unicode(text)
    if not data.endswith(u"\n"):
        data = data + u"\n"
    sys.stdout.write(data.encode("utf-8", "ignore"))


def decode_best(data):
    for encoding in ("utf-8", "gbk"):
        try:
            return data.decode(encoding)
        except Exception:  # pylint: disable=broad-except
            continue
    return data.decode("utf-8", "ignore")


def strip_include_lines(text):
    lines = text.splitlines()
    result = []
    pending_blank = False
    for line in lines:
        stripped = line.strip()
        if stripped.startswith("#include"):
            pending_blank = True
            continue
        if pending_blank and stripped == "":
            continue
        pending_blank = False
        result.append(line)
    while result and result[-1].strip() == "":
        result.pop()
    return result


def load_remote_file(rel_path):
    target = "origin/main:%s" % rel_path.replace("\\", "/")
    result = subprocess.Popen(
        ["git", "show", target],
        cwd=REPO_ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, _ = result.communicate()
    if result.returncode != 0:
        raise IOError(rel_path)
    text = decode_best(stdout)
    return strip_include_lines(text)


def load_local_file(path):
    with open(path, "rb") as fh:
        data = fh.read()
    text = decode_best(data)
    return strip_include_lines(text)


def collect_files():
    file_map = {}
    for root, _, files in os.walk(CLASSES_DIR):
        for name in files:
            full_path = os.path.join(root, name)
            file_map.setdefault(name, []).append(full_path)
    return file_map


def main():
    if not os.path.isdir(CLASSES_DIR):
        print("Classes directory not found at %s" % CLASSES_DIR)
        return 1

    file_map = collect_files()
    duplicates = {name: paths for name, paths in file_map.items() if len(paths) > 1}
    if duplicates:
        safe_print("Duplicate filenames detected; cannot build 1:1 mapping:")
        for name, paths in sorted(duplicates.items()):
            for path in paths:
                rel = os.path.relpath(path, REPO_ROOT)
            safe_print("  %s: %s" % (name, rel))
        return 2

    differences = []
    missing_remote = []

    for name, paths in sorted(file_map.items()):
        file_path = paths[0]
        rel_new = os.path.relpath(file_path, REPO_ROOT)
        old_rel = os.path.join("Classes", name).replace("\\", "/")

        try:
            old_lines = load_remote_file(old_rel)
        except IOError:
            missing_remote.append(rel_new)
            continue

        new_lines = load_local_file(file_path)
        if old_lines != new_lines:
            diff = list(
                difflib.unified_diff(
                    old_lines,
                    new_lines,
                    fromfile="origin/main:%s" % old_rel,
                    tofile=rel_new.replace("\\", "/"),
                    lineterm="",
                )
            )
            differences.append((rel_new, diff))

    if missing_remote:
        safe_print("Files without remote match (skipped):")
        for path in missing_remote:
            safe_print("  %s" % path)

    if differences:
        with io.open(LOG_PATH, "w", encoding="utf-8") as log_file:
            for path, diff in differences:
                title = u"=== %s ===\n" % path.replace("\\", "/")
                log_file.write(title)
                for line in diff:
                    log_file.write(_to_unicode(line) + u"\n")
                log_file.write(u"\n")
        safe_print(
            "Found %d files with content differences (excluding #include lines). Details saved to %s"
            % (len(differences), LOG_PATH)
        )
        return 3

    safe_print("No differences detected outside #include lines.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
