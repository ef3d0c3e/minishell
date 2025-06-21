# Generates completion files for the shell, using fish's completion files
# USAGE: python3 fish2shell.py /usr/share/fish/completions/ completion
import os
import shlex
import sys

def parse_fish_completions(lines, progname):
    output = []
    skip_block = False

    for line in lines:
        stripped = line.strip()

        if stripped.startswith(("for ", "while ", "if ")):
            skip_block = True
        elif stripped == "end":
            skip_block = False
        elif skip_block or not stripped.startswith("complete "):
            continue
        else:
            stripped = stripped.replace("$progname", progname).replace("${progname}", progname)

            try:
                parts = shlex.split(stripped)
            except ValueError:
                continue  # Skip malformed lines
            if "-c" not in parts:
                continue

            # Retain only relevant flags
            new_parts = ["complete"]
            i = 1
            found = {"-s": False, "-l": False, "-d": False}
            while i < len(parts):
                if parts[i] == "-c":
                    new_parts += ["-c", parts[i+1]]
                    i += 2
                elif parts[i] in ("-s", "-l"):
                    new_parts += [parts[i], "'" + parts[i+1].replace("'", "'\"'\"'") + "'"]
                    found[parts[i]] = True
                    i += 2
                elif parts[i] == "-d":
                    desc = parts[i+1].replace("'", "'\"'\"'")
                    new_parts += ["-d", f"'{desc}'"]
                    found["-d"] = True
                    i += 2
                else:
                    i += 1

            if any(found.values()):
                output.append(" ".join(new_parts))

    return output

def convert_folder(input_dir, output_dir):
    os.makedirs(output_dir, exist_ok=True)

    for filename in os.listdir(input_dir):
        if not filename.endswith(".fish"):
            continue

        progname = os.path.splitext(filename)[0]
        input_path = os.path.join(input_dir, filename)
        output_path = os.path.join(output_dir, filename.replace(".fish", ".sh"))

        with open(input_path, "r") as infile:
            lines = infile.readlines()

        parsed_lines = parse_fish_completions(lines, progname)

        if parsed_lines:
            with open(output_path, "w") as outfile:
                outfile.write("\n".join(parsed_lines) + "\n")

        print(f"Processed {filename} -> {os.path.basename(output_path)}")

# Usage: python script.py input_dir output_dir
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_dir> <output_dir>")
        sys.exit(1)

    convert_folder(sys.argv[1], sys.argv[2])
