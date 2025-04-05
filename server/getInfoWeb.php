<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$data = "";

if($_GET['file'] == 'sys'){
$data = file_get_contents('../infoFiles/sysInfo.txt');
}
else if ($_GET['file'] == 'cmd'){
$data = file_get_contents('../infoFiles/cmd.txt');
}
else if($_GET['file'] == 'autoIrr'){
    $data = file_get_contents('../infoFiles/autoIrrInfo.txt');
}
echo $data;


?> 