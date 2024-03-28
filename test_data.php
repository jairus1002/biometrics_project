<?php

$hostname = "localhost";
$username = "root";
$password = "";
$database = "esp";

// Connect to MySQL database
$conn = mysqli_connect($hostname, $username, $password, $database);

// Check if connection was successful
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

// Check if data is received via POST method
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Decode JSON data
    $data = json_decode(file_get_contents("php://input"), true);

    // Extract temperature and humidity from JSON data
    $temperature = $data["temperature"];
    $humidity = $data["humidity"];

    // SQL query to insert data into the 'fingerprint' table
    $sql = "INSERT INTO ep (temperature, humidity) VALUES ('$temperature', '$humidity')";

    if (mysqli_query($conn, $sql)) {
        echo "Data inserted successfully";
    } else {
        echo "Error: " . $sql . "<br>" . mysqli_error($conn);
    }
} else {
    echo "No data received";
}

// Close the database connection
mysqli_close($conn);

?>
