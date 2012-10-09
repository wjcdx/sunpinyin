#include <locale.h>
#include <stdlib.h>
#include <unistd.h>

#include "trie.h"
#include "trie_factory.h"
#include "maker_factory.h"
#include "trie_writer.h"
#include "CUnigramSorter.h"

/**
 * This program is used to generate the PINYIN Lexicon. It
 * Only works on zh_CN.utf8 locale.\n
 * args:
 *    -# dictionary file, in utf8 encoding, line-based text file,
 *       each line looks like\n
 *       CCC  id  [pinyin'pinyin'pinyin]*
 *    -# output binary PINYIN Lexicon file name
 *    -# log file to print the generated PINYIN Lexicon
 *    -# language model to sort the words of each node
 */
void
ShowUsage(const char* progname)
{
    fprintf(
        stderr,
        "Usage:\n"
        "    %s -i lexicon_file -o result_file -l log_file -s slm_file [-e le|be] -t py|xh\n",
        progname);
    fprintf(
        stderr,
        "Description:\n"
        "    This program is used to generate the Lexicon(py for PINYIN, while xh for XINGHUA).\n"
        "    It Only works on zh_CN.utf8 locale\n"
        "\n");
    exit(100);
}

static CTrie::TrieType parse_type(const char *type)
{
    if (strcmp(type, "py") == 0) {
        return CTrie::PINYIN;
    } else if (strcmp(type, "xh") == 0) {
        return CTrie::XINGHUA;
    }
    return CTrie::INVALID;
}

int
main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    const char* lexicon_file = NULL;
    const char* result_file = NULL;
    const char* log_file = NULL;
    const char* slm_file = NULL;
    int build_endian = get_host_endian();
    int opt;
    CTrie::TrieType type;
    while ((opt = getopt(argc, argv, "i:o:l:s:e:t:")) != -1) {
        switch (opt) {
        case 'i':
            lexicon_file = optarg;
            break;
        case 'o':
            result_file = optarg;
            break;
        case 'l':
            log_file = optarg;
            break;
        case 's':
            slm_file = optarg;
            break;
        case 'e':
            build_endian = parse_endian(optarg);
            break;
        case 't':
            type = parse_type(optarg);
            break;
        }
    }
    if (!lexicon_file || !result_file || !log_file || !slm_file ||
        build_endian == -1 || type == CTrie::INVALID) {
        ShowUsage(argv[0]);
    }

    printf("Opening language model..."); fflush(stdout);
    CUnigramSorter srt;
    if (!srt.open(slm_file)) {
        printf("error!\n");
        return -1;
    }
    printf("done!\n"); fflush(stdout);

    CUnitData::initialize(type);
    CTrieMaker *maker = MakerFactory::getMaker(type);

    maker->constructFromLexicon(lexicon_file);

    printf("Writing out..."); fflush(stdout);
    maker->write(result_file, &srt, get_host_endian() != build_endian);
    printf("done!\n"); fflush(stdout);

    srt.close();

    if (get_host_endian() != build_endian) {
        printf("host endian is different from build endian. "
               "log_file will not be written.\n");
        fflush(stdout);
        return 0;
    }

    printf("Printing the lexicon out to log_file..."); fflush(stdout);
    CTrie *t = TrieFactory::getTrie(type);
    t->load(result_file);

    FILE *fp = fopen(log_file, "w");
    t->print(fp);
    fclose(fp);

    printf("done!\n");

    return 0;
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
