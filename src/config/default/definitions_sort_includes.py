input_file = "definitions.h"
output_file = "definitions.h"

header_marker = [
    "// *****************************************************************************\n",
    "// *****************************************************************************\n",
    "// Section: Included Files\n",
    "// *****************************************************************************\n",
    "// *****************************************************************************\n"
]

with open(input_file, "r") as f:
    lines = f.readlines()

# Find the marker position
marker_idx = 0
for i in range(len(lines)):
    if lines[i:i+5] == header_marker:
        marker_idx = i + 5
        break

# Split the file into three parts
before = lines[:marker_idx]
rest = lines[marker_idx:]

# Separate includes and other lines after the marker
include_lines = [line for line in rest if line.strip().startswith("#include")]
other_lines = [line for line in rest if not line.strip().startswith("#include")]

include_lines.sort()

with open(output_file, "w") as f:
    f.writelines(before)
    f.writelines(include_lines)
    f.writelines(other_lines)