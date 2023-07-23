from datetime import date
from os import mkdir
import json

OUT_DIR = "_site/"
RESULTS_PATH = "utils/result.json"

INDEX_TEMPLATE = """<html><head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<title>GUI ToolKit Benchmarks</title>
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
th:first-of-type {
    cursor: default
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
  <br><br>
  <img src="https://img.shields.io/github/stars/IronOxidizer/gui-toolkit-benchmarks"/>
  <img src="https://img.shields.io/github/actions/workflow/status/IronOxidizer/gui-toolkit-benchmarks/main.yml"/>
  <img src="https://img.shields.io/github/issues/IronOxidizer/gui-toolkit-benchmarks"/>
</header>
<br>
<table>
  <tr>
    <th>#</th>
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
const tbody = document.getElementsByTagName("tbody")[0];
const headers = document.getElementsByTagName("th");
var lastIndex = 6; // Default sort by memory
var sortDirection = 1;
headers[lastIndex].style = "color: #fdb735"
for (let i = 1; i < headers.length; ++i)
	headers[i].addEventListener('click', function() {sortColumn(i);});


function sortColumn(index) {
	sortDirection = index === lastIndex ? sortDirection * -1 : 1;
    headers[lastIndex].style = "color: #ddd"
    headers[index].style = "color: #fdb735"
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
    Array.from(tbody.getElementsByTagName("tr")).slice(1).map((row, i) => row.children[0].innerText = i+1);
}
</script>"""

DEVICE_TEMPLATE = """<tr>
<td>{i}</td>
<td>{name}</td>
<td>{lang}</td>
<td>{mode}</td>
<td>{platform_lib}</td>
<td>{startup}</td>
<td>{memory}</td>
<td>{executable_size}</td>
<td>{dependencies_size}</td>
</tr>"""

with open(RESULTS_PATH, 'r') as f:
  results = json.load(f)
results.sort(key=lambda toolkit: toolkit["memory"]) # default sort by lowest memory

html_rows = "".join(DEVICE_TEMPLATE.format(i=i+1, **result) for i, result in enumerate(results))
html_index = INDEX_TEMPLATE % (date.today().strftime("%B %d, %Y"), html_rows)
mkdir(OUT_DIR)
open(OUT_DIR + "index.html", "w").write(html_index)
