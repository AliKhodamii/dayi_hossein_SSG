tableData = [];

function formatMinutes(minutes) {
  const hours = Math.floor(minutes / 60);
  const mins = minutes % 60;
  return `${hours}:${mins.toString().padStart(2, "0")}`;
}

httpReqAndCreateRecIrrTable();

function httpReqAndCreateRecIrrTable() {
  const Http = new XMLHttpRequest();
  const url =
    "https://sed-smarthome.ir/dayi_hossein/server/getIrrRec.php/?request=recentIrr";
  Http.open("GET", url);
  Http.send();

  Http.onreadystatechange = (e) => {
    // console.log(Http.responseText);
    tableData = JSON.parse(Http.responseText);
    // console.log(tableData);
    createTable();
  };
}
function createTable() {
  var html = "";
  for (var i = 0; i < tableData.length; i++) {
    let time = "";
    time = formatMinutes(Number(tableData[i].irr_duration));
    html += "<tr>";

    html += "<td>" + (i + 1).toString() + "</td>";
    html += "<td>" + tableData[i].farsiDay + "</td>";
    html += "<td>" + tableData[i].date + "</td>";
    html += "<td>" + tableData[i].time + "</td>";
    html += "<td>" + time + "</td>";

    html += "</tr>";
  }
  document.getElementById("recIrr").innerHTML = html;
}
