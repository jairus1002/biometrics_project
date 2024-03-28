<?php
$server_name = "localhost";
$username = "root";
$password = "";
$database_name = "user";

// Connect to MySQL database
$conn = mysqli_connect($server_name, $username, $password, $database_name);

// Check if connection was successful
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

// Query to fetch user details from the database
$sql = "SELECT * FROM registration";

// Execute the query
$result = mysqli_query($conn, $sql);

// Check if any rows were returned
if (mysqli_num_rows($result) > 0) {
    // Array to store user details
    $users = array();

    // Fetch rows from the result set
    while ($row = mysqli_fetch_assoc($result)) {
        // Add each user's details to the array
        $users[] = $row;
    }

    // Return user details as JSON
    header('Content-Type: application/json');
    echo json_encode($users);
} else {
    // No users found
    echo json_encode(["error" => "No users found"]);
}

// Close the database connection
mysqli_close($conn);
?>
