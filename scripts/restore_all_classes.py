import io
import os
import subprocess

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CLASSES_DIR = os.path.join(REPO_ROOT, "Classes")
CACHE = {}


def decode_text(data):
    try:
        return data.decode("utf-8")
    except UnicodeDecodeError:
        return data.decode("gbk", "ignore")


def git_show(basename):
    if basename in CACHE:
        return CACHE[basename]
    target = "origin/main:Classes/{0}".format(basename)
    proc = subprocess.Popen(
        ["git", "show", target],
        cwd=str(REPO_ROOT),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = proc.communicate()
    if proc.returncode != 0:
        msg = stderr.decode('utf-8', 'ignore')
        raise RuntimeError("git show failed for {0}: {1}".format(target, msg))
    text = decode_text(stdout)
    CACHE[basename] = text
    return text


def read_local(path):
    with io.open(path, "r", encoding="utf-8", errors="ignore") as fh:
        return fh.read()


def write_local(path, text):
    with io.open(path, "w", encoding="utf-8") as fh:
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
        return remote_text if remote_text.endswith("\n") else remote_text + "\n"

    r_start, r_end = remote_block
    l_start, l_end = local_block

    merged_lines = []
    merged_lines.extend(remote_lines[:r_start])
    merged_lines.extend(local_lines[l_start:l_end])
    merged_lines.extend(remote_lines[r_end:])

    merged_text = "\n".join(merged_lines)
    if not merged_text.endswith("\n"):
        merged_text += "\n"
    return merged_text


def should_process(path):
    _, ext = os.path.splitext(path)
    if ext.lower() not in {".cpp", ".h"}:
        return False
    return True


def main():
    if not os.path.isdir(CLASSES_DIR):
        raise SystemExit("Classes directory not found at {0}".format(CLASSES_DIR))

    updated = []
    skipped = []
    for root, _, files in os.walk(CLASSES_DIR):
        for name in files:
            file_path = os.path.join(root, name)
            if not should_process(file_path):
                continue
            basename = name
            try:
                remote_text = git_show(basename)
            except RuntimeError:
                rel_path = os.path.relpath(file_path, REPO_ROOT)
                skipped.append(rel_path)
                continue

            local_text = read_local(file_path)
            merged_text = merge_content(remote_text, local_text)
            if merged_text != local_text:
                write_local(file_path, merged_text)
                updated.append(os.path.relpath(file_path, REPO_ROOT))

    print("Updated {0} files".format(len(updated)))
    if skipped:
        print("Skipped files (no remote counterpart):")
        for rel in skipped:
            print("  {0}".format(rel))


def entry():
    try:
        main()
    except Exception as exc:  # pylint: disable=broad-except
        print("Error: {0}".format(exc))
        raise


if __name__ == "__main__":
    entry()
