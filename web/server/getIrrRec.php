<?php
require_once('./lib/jdatetime.class.php');

// db connection info
$hostname = 'localhost:3306';
$username = 'jjqioyps_dayihossein';
$password = 'Sed1508libero';
$database = 'jjqioyps_dayi_hossein_ssg';

// Create connection
$conn = new mysqli($servername, $username, $password, $database);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if($_GET['request'] == 'recentIrr'){

$sql = "  SELECT * FROM `irr_rec` ORDER BY `irr_start_datetime` DESC limit 5  ";
$result = $conn->query($sql);

$data = array();

//put result in a array
if ($result->num_rows > 0) {


    while ($row = $result->fetch_assoc()) {
        $row["date"] = (new jDateTime(false, true))->convertFormatToFormat('d / F ', 'Y-m-d H:i:s', $row["irr_start_datetime"]);
        $row["farsiDay"] = (new jDateTime(false, true))->convertFormatToFormat('l', 'Y-m-d H:i:s', $row["irr_start_datetime"]);
        $row["time"] = (new jDateTime(false, true))->convertFormatToFormat('H:i', 'Y-m-d H:i:s', $row["irr_start_datetime"]);

        $data[] = $row;
    }
}

$conn->close();
header('Content-Type: application/json');
echo json_encode($data);
}

if($_GET['request'] == 'nextIrrDate'){
    $sql = "SELECT * FROM irr_rec ORDER BY irr_start_datetime DESC LIMIT 1";

    $result = $conn->query($sql);
    $rows = array();

    if($result->num_rows >0){
        while($row = $result->fetch_assoc()){
            $rows[]=$row;
        }
    }

    $autoIrrInfoJson = file_get_contents("../infoFiles/autoIrrInfo.txt");
    $autoIrrInfo = json_decode($autoIrrInfoJson);
    $howOften = $autoIrrInfo ->howOften;


    $lastIrrDatetime = new DateTime ($rows[0]['irr_start_datetime']);
    $lastIrrDatetime->add(new DateInterval("P{$howOften}D"));
    $nextIrrDate = $lastIrrDatetime->format('Y-m-d');
    
    $rows[0]["nextIrrDate"] =  (new jDateTime(false, true))->convertFormatToFormat('d / m / Y', 'Y-m-d', $nextIrrDate );

    $conn->close();
    header('Content-Type: application/json');
    echo json_encode($rows);

}