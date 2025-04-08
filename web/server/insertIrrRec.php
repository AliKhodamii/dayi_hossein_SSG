<?php
date_default_timezone_set('Asia/Tehran');
if($_POST["insertIrrRec"] != null){

    // get duration from ESP data
    $sysInfoJson = $_POST["insertIntoDB"];
    $sysInfo = json_decode($sysInfoJson);

    //create date of irr
    $newDatetime = date('Y-m-d H:i:s');
    $duration = (int)$sysInfo->duration;

    //connect info
    $hostname = 'localhost:3306';
    $username = 'jjqioyps_dayihossein';
    $password = 'Sed1508libero';
    $database = 'jjqioyps_dayi_hossein_ssg';

    // Create a database connection
    $conn = new mysqli($hostname, $username, $password, $database);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }



    // Prepare the SQL query
    $sql = "INSERT INTO irr_rec (irr_duration, irr_start_datetime, irr_end_datetime) VALUES ('$duration', '$newDatetime', '')";

    // Execute the SQL query
    if ($conn->query($sql) === TRUE) {
        echo "Data inserted successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    // Close the database connection
    $conn->close();
}