<?php

function tripal_quick_fasta_fetch_features_for_sequence() {

  $query = db_select('chado.feature', 'f');
  $query->join('chado.cvterm', 'cvt', 'f.type_id = cvt.cvterm_id');
  $results = $query->fields('cvt', ['cvterm_id', 'name'])
    ->distinct()
    ->execute()
    ->fetchAll();

  $types = [];
  foreach ($results as $result) {
    $types[$result->cvterm_id] = $result->name;
  }

  return $types;
}
