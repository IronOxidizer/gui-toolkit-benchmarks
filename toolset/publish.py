from datetime import date
from os import mkdir
import json

OUT_DIR = "_site/"
RESULTS_PATH = "toolset/result.json"

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
th { cursor: pointer }
tr:nth-child(even) { background: #444 }
header { text-align: center }
table { margin: auto }
</style>
</head>

<header>
  <a href="https://github.com/IronOxidizer/gui-toolkit-benchmarks">Help improve this open source project on Github</a>. 
  This page was last updated on %s
</header>
<table>
  <tr>
    <th>Toolkit</th>
    <th>Memory Usage (KB)</th>
    <th>Startup Time (ms)</th>
  </tr>
  %s
</table> 
<script>
"use strict";
var lastIndex = -1, sortDirection = 1;
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
			const cellA = rowA.children[index].innerText;
			const cellB = rowB.children[index].innerText;
			return ((cellA > cellB) - (cellB > cellA)) * sortDirection;
		}
	))
}
</script>"""

DEVICE_TEMPLATE = """<tr>
<td>{name}</td>
<td>{memory}</td>
<td>{startup}</td>
</tr>"""

with open(RESULTS_PATH, 'r') as f:
  results = json.load(f)

html_rows = "".join(DEVICE_TEMPLATE.format(**result) for result in results)
html_index = INDEX_TEMPLATE % (date.today().strftime("%B %d, %Y"), html_rows)
mkdir(OUT_DIR)
open(OUT_DIR + "index.html", "w").write(html_index)
