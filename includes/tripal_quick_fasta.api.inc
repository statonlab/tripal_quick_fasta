<?php

/**
 * @throws \Exception
 */
function tripal_quick_run_file_job()
{

    //open file
    // $uri = 'public://sitewide_proteins_with_mRNA_names.FASTA';
    $path = 'public://sitewide_proteins_with_mRNA_names.FASTA';
    if (file_exists(drupal_realpath($path))) {
        unlink(drupal_realpath($path));
    }

    $prot_term = chado_get_cvterm(['id' => 'SO:0000104']);
    $query = db_select('chado.feature', 'f')
        ->fields('f', ['feature_id'])
        ->condition('f.type_id', $prot_term->cvterm_id)
        ->orderBy('feature_id', 'ASC');

    $count = $query->countQuery()->execute()->fetchField();

    $n = 500;
    $offset = 0;
    print("creating FASTA file for $count sequences.  This may take a while\n");
    while ($offset < $count) {

        if ($offset % ($n * 20) == 0) {
            print ("sequence number ".$offset."\n");
        }
        $chunk = $query->range($offset, $n)->execute()->fetchAll();

        tripal_quick_get_protein_seqs_mRNA_names($chunk, $path);
        $offset += $n;
    }
}

/**
 * @param $protein_feature_chunk
 * @param $path
 * @throws \Exception
 */
function tripal_quick_get_protein_seqs_mRNA_names($protein_feature_chunk, $path)
{
    $fh = fopen(drupal_realpath($path), 'a');

    if ($fh === false) {
        throw new Exception('Unable to open file at path: '.drupal_realpath($path));
    }

    foreach ($protein_feature_chunk as $item) {
        $feature_id = $item->feature_id;
        try {
            $seq = tripal_quick_fasta_fetch_seq($feature_id);
            $name = tripal_quick_fasta_fetch_parent_mRNA($feature_id);
            if (empty($name)) {
                print "Warning: feature ID $feature_id has no parent. Skipped!";
                continue;
            }
            fwrite($fh, ">".$name."\n".$seq."\n");
        } catch (Exception $e) {
            print $e->getMessage();

            return;
        }
    }
    fclose($fh);
}

function tripal_quick_fasta_fetch_features_for_sequence()
{

    $query = db_select('chado.feature', 'f');
    $query->join('chado.cvterm', 'cvt', 'f.type_id = cvt.cvterm_id');
    $results = $query->fields('cvt', ['cvterm_id', 'name'])->distinct()->execute()->fetchAll();

    $types = [];
    foreach ($results as $result) {
        $types[$result->cvterm_id] = $result->name;
    }

    return $types;
}

/**
 * given a feature, fetch the parent's uniquename.
 * note: feature should only have one parent of type mRNA and/or mrna_contig
 *
 * @param $feature_id
 *
 * @return mixed
 */
function tripal_quick_fasta_fetch_parent_mRNA($feature_id)
{
    $mrna_term = chado_get_cvterm(['id' => 'SO:0000234'])->cvterm_id;
    $mrna_contig_term = chado_get_cvterm(['id' => 'SO:0001829'])->cvterm_id;

    $query = db_select('chado.feature_relationship', 'fr');
    $query->join('chado.feature', 'f', 'fr.object_id = f.feature_id');
    $query->fields('f', ['uniquename']);
    $query->condition('fr.subject_id', $feature_id);
    $query->condition('f.type_id', [$mrna_term, $mrna_contig_term], 'IN');
    $result = $query->execute()->fetchField();

    return $result;
}

function tripal_quick_fasta_fetch_seq($feature_id)
{

    $result = db_select('chado.feature', 'f')
        ->fields('f', ['residues'])
        ->condition('f.feature_id', $feature_id)
        ->execute()
        ->fetchField();

    if (! $result) {

    }

    return $result;
}
