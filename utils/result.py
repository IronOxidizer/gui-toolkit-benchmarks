import json
with open("utils/result.json", mode="r", encoding="utf-8") as f:
    old_results = json.load(f)

for toolkit in old_results:
    with open(f"toolkits/{toolkit['lang']}/{toolkit['name']}/metadata.json", mode="r", encoding="utf-8") as f:
        metadata = json.load(f)
        toolkit["website"] = metadata["website"]

with open("new_results.json", mode="w", encoding="utf-8") as f:
    json.dump(old_results, f)
