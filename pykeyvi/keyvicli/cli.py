import json
import pykeyvi

from argparse import ArgumentParser


def stats(input_file):
    print json.dumps(pykeyvi.Dictionary(input_file).GetStatistics(), indent=4, sort_keys=True)


def dump(input_file, output_file):
    dictionary = pykeyvi.Dictionary(input_file)
    with open(output_file, 'w') as file_out:
        for key, value in dictionary.GetAllItems():
            file_out.write(key)
            if value:
                file_out.write('\t' + value)
            file_out.write('\n')


def compile(input_file, output_file, dict_type):
    if dict_type == 'json':
        dictionary = pykeyvi.JsonDictionaryCompiler()
    elif dict_type == 'key-only':
        dictionary = pykeyvi.KeyOnlyDictionaryCompiler()
    else:
        return 'Must never reach here'

    with open(input_file) as file_in:
        for line in file_in:
            line = line.rstrip('\n')
            try:
                splits = line.split('\t')
                if dict_type == 'key-only':
                    dictionary.Add(splits[0])
                else:
                    dictionary.Add(splits[0], splits[1])
            except:
                print 'Can not parse line: {}'.format(line)

    dictionary.Compile()
    dictionary.WriteToFile(output_file)


def main():
    argument_parser = ArgumentParser(description='keyvi')
    subparsers = argument_parser.add_subparsers(dest='command')

    stats_parser = subparsers.add_parser('stats')
    stats_parser.add_argument('input_file', type=str, metavar='FILE')

    dump_parser = subparsers.add_parser('dump')
    dump_parser.add_argument('input_file', type=str, metavar='FILE')
    dump_parser.add_argument('output_file', type=str, metavar='OUT_FILE')

    compile_parser = subparsers.add_parser('compile')
    compile_parser.add_argument('input_file', type=str, metavar='FILE')
    compile_parser.add_argument('output_file', type=str, metavar='OUT_FILE')
    compile_parser.add_argument('dict_type', type=str, choices=['json', 'key-only'],
                                help='dictionary type')

    args = argument_parser.parse_args()

    if args.command == 'stats':
        stats(args.input_file)
    elif args.command == 'dump':
        dump(args.input_file, args.output_file)
    elif args.command == 'compile':
        compile(args.input_file, args.output_file, args.dict_type)
    else:
        return 'Must never reach here'


if __name__ == '__main__':
    main()