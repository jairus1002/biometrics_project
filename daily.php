<?php
// Database connection parameters
$servername = "localhost"; // Change this to your database server name
$username = "root"; // Change this to your database username
$password = ""; // Change this to your database password
$dbname = "user"; // Change this to your database name

// Connect to the database
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Fetch the latest fingerprint ID from the f_id table
$sql_latest_id = "SELECT fingerprint_id FROM f_id ORDER BY id DESC LIMIT 1";
$result_latest_id = $conn->query($sql_latest_id);

if ($result_latest_id && $result_latest_id->num_rows > 0) {
    $row_latest_id = $result_latest_id->fetch_assoc();
    $latest_fingerprint_id = $row_latest_id['fingerprint_id'];

    // Retrieve user information based on fingerprint ID
    $sql_user_info = "SELECT first_name, second_name, email FROM registration WHERE fingerprint_status = ?";
    $stmt = $conn->prepare($sql_user_info);
    $stmt->bind_param("i", $latest_fingerprint_id);
    $stmt->execute();
    $result_user_info = $stmt->get_result();

    if ($result_user_info && $result_user_info->num_rows > 0) {
        $row_user_info = $result_user_info->fetch_assoc();
        $first_name = $row_user_info['first_name'];
        $second_name = $row_user_info['second_name'];
        $email = $row_user_info['email'];

        // Store the extracted data into the d_records table
        $insert_sql = "INSERT INTO d_records (first_name, second_name, email) VALUES (?, ?, ?)";
        $stmt = $conn->prepare($insert_sql);
        $stmt->bind_param("sss", $first_name, $second_name, $email);
        if ($stmt->execute()) {
            echo "Data inserted into d_records successfully.";
        } else {
            echo "Error inserting data into d_records: " . $stmt->error;
        }
    } else {
        echo "No user found with the latest fingerprint ID";
    }
} else {
    echo "No fingerprint ID found in the database";
}

// Close the database connection
$conn->close();
?>
