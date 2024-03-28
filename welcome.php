<?php
// Database connection parameters
$servername = "localhost"; // Change this to your database server name
$username = "root"; // Change this to your database username
$password = ""; // Change this to your database password
$dbname = "user"; // Change this to your database name

// Check if data is received via POST method
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Decode JSON data
    $data = json_decode(file_get_contents("php://input"), true);

    // Check if the required parameter 'id' exists
    if (isset($data['id'])) {
        // Retrieve the ID from JSON data
        $id = $data['id'];

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);

        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        // Prepare SQL statement to insert the ID into the database table
        $sql_insert = "INSERT INTO f_id (fingerprint_id) VALUES ('$id')";

        if ($conn->query($sql_insert) === TRUE) {
            echo "Fingerprint ID inserted successfully into the database";
        } else {
            echo "Error storing ID: " . $conn->error;
        }

        // Close database connection
        $conn->close();
    } else {
        // "id" parameter not provided in the JSON data
        echo 'ID parameter missing';
    }
} else {
    // No data received via POST method
    echo "No data received";
}
?>
