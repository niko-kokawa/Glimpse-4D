import json
import struct
import os

ROOT_DIRECTORY = r"D:\VS Projects\Glimpse 4D\Glimpse 4D"
path = os.path.join(ROOT_DIRECTORY, "4D Objects")

for filename in os.listdir(path):
    if not filename.endswith(".json"):
        continue

    json_path = os.path.join(path, filename)
    bin_path = json_path.replace(".json", ".bin")

    # ---- READ JSON ----
    with open(json_path, "r") as f:
        data = json.load(f)

    # ---- WRITE BINARY ----
    with open(bin_path, "wb") as f:
        f.write(struct.pack("I", len(data["vertices"])))

        for v in data["vertices"]:
            f.write(struct.pack("4f", *v))  # <-- FLOATS

        f.write(struct.pack("I", len(data["edges"])))
        for e in data["edges"]:
            f.write(struct.pack("2I", *e))  # indices should be unsigned