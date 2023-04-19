from datetime import date
from os import mkdir
import json

OUT_DIR = "_site/"
RESULTS_PATH = "utils/result.json"

INDEX_TEMPLATE = """<head>
<style>
body {
    font-family: roboto,helvetica,arial,sans-serif;
    background-color:#2a2a2a;
    color:#ddd;
    font-weight:400
}
a:hover, a:focus {
    color: #d2991d;
    text-decoration: underline
}
a {
    color:#fdb735;
    font-weight:500;
    text-decoration-line:none
}
th {
    cursor: pointer;
    padding: 0 10px
}
td {
    text-align: right;
    padding: 0 10px
}
tr:nth-child(even) { background: #444 }
header { text-align: center }
table { 
    margin: auto;
    background-color: #505050
}
</style>
</head>

<header>
  <a href="https://github.com/IronOxidizer/gui-toolkit-benchmarks">Help improve this open source project on Github</a>. 
  This page was last updated on %s
</header>
<br>
<table>
  <tr>
    <th>Toolkit</th>
    <th>Language</th>
    <th>Mode</th>
    <th>Platform Library</th>
    <th>Memory Usage (KB)</th>
    <th>Startup Time (ms)</th>
    <th>Executable Size (KB)</th>
    <th>Dependencies Size (KB)</th>
  </tr>
  %s
</table> 
<script>
"use strict";
var lastIndex = 4; // Default sort by memory
var sortDirection = 1;
const tbody = document.getElementsByTagName("tbody")[0];
const headers = document.getElementsByTagName("th");
for (let i = 0; i < headers.length; ++i)
	headers[i].addEventListener('click', function() {sortColumn(i);});

function sortColumn(index) {
	sortDirection = index === lastIndex ? sortDirection * -1 : 1;
	lastIndex = index;
	const oldRows = Array.from(tbody.getElementsByTagName("tr"));
	tbody.replaceChildren(oldRows[0],
		...oldRows.slice(1).sort(function (rowA, rowB) {
			let cellA = rowA.children[index].innerText;
			let cellB = rowB.children[index].innerText;
                        if (!isNaN(parseFloat(cellA)) && !isNaN(parseFloat(cellB))) {
                            cellA = parseFloat(cellA);
                            cellB = parseFloat(cellB);
                        }
			return ((cellA > cellB) - (cellB > cellA)) * sortDirection;
		}
	))
}
</script>"""

DEVICE_TEMPLATE = """<tr>
<td>{name}</td>
<td>{lang}</td>
<td>{mode}</td>
<td>{platform_lib}</td>
<td>{memory}</td>
<td>{startup}</td>
<td>{executable_size}</td>
<td>{dependencies_size}</td>
</tr>"""

with open(RESULTS_PATH, 'r') as f:
  results = json.load(f)
results.sort(key=lambda toolkit: toolkit["memory"]) # default sort by lowest memory

html_rows = "".join(DEVICE_TEMPLATE.format(**result) for result in results)
html_index = INDEX_TEMPLATE % (date.today().strftime("%B %d, %Y"), html_rows)
mkdir(OUT_DIR)
open(OUT_DIR + "index.html", "w").write(html_index)
