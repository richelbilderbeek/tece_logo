
get_group_name <- function()
{
  return (paste(get_group_names))
}

# http://www.rug.nl/research/gelifes/tres/_etienne/research
get_group_names <- function()
{
  return (c("Theoretical", "Evolutionary", "Community", "Ecology"))
}



create_alignment <- function()
{
  fasta_filename <- "test.fasta"
  save_text(fasta_filename, create_fasta_text())
  phy_dat <- phangorn::read.phyDat(file = fasta_filename, format = "fasta", type = "DNA")
  dm  <- phangorn::dist.ml(phy_dat)
  phylogeny <- phangorn::upgma(dm)
  ape::plot.phylo(phylogeny, cex = 3.0)
  ape::nodelabels("&", 7, cex = 4.0)
  ape::plot.phylo(ape::makeNodeLabel(phylogeny, method = "number", prefix = "Node", nodeList = list(5,6,7,8)))

}

create_fasta_text <- function(
  sequences = c("aag", "aat", "acg", "act")
)
{

  fasta_text <- c(
    paste0(">", get_group_names()[4]),
    sequences[4],
    paste0(">", get_group_names()[3]),
    sequences[3],
    paste0(">", get_group_names()[2]),
    sequences[2],
    paste0(">", get_group_names()[1]),
    sequences[1]
  )
  return (fasta_text)
}


#' Save text (a container of strings) to a file
save_text <- function(filename, text) {
  my_file <- file(filename)
  writeLines(text, my_file)
  close(my_file)
}
