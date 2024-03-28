<?php
// generate_pdf.php

// Include the TCPDF library
require_once('tcpdf/tcpdf.php');

// Create a function to generate and output the PDF
function generatePDF() {
    // Create new PDF document
    $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

    // Set document information
    $pdf->SetCreator('MUSUNDI JAIRUS');
    $pdf->SetAuthor('KIBISU');
    $pdf->SetTitle('Daily Records');
    $pdf->SetSubject('Daily Records');
    $pdf->SetKeywords('Daily, Records');

    // Add a page
    $pdf->AddPage();

    // Set content
    $pdf->SetFont('helvetica', '', 12);
    $pdf->Cell(0, 10, 'Daily Records', 0, 1);
    // Add more content as needed

    // Output PDF as response
    $pdf->Output('daily_records.pdf', 'I');
}

// Call the function to generate and output the PDF
generatePDF();
