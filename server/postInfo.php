<?php

if ($_POST["sysInfo"] != null){
    $sysInfo = json_decode($_POST["sysInfo"]);
    $sysInfoJson = json_encode($sysInfo,JSON_PRETTY_PRINT);
    file_put_contents("../infoFiles/sysInfo.txt",$sysInfoJson);
}
if($_POST["cmdInfo"] != null){
    $cmdInfo = json_decode($_POST["cmdInfo"]);
    $cmdInfoJson = json_encode($cmdInfo,JSON_PRETTY_PRINT);
    file_put_contents("../infoFiles/cmd.txt",$cmdInfoJson);
}
if($_POST["autoIrrInfo"]){
    $autoIrrInfo = json_decode($_POST["autoIrrInfo"]);
    $autoIrrInfoJson = json_encode($autoIrrInfo,JSON_PRETTY_PRINT);
    file_put_contents("../infoFiles/autoIrrInfo.txt",$autoIrrInfoJson);
}