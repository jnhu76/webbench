package main

import (
	"flag"
	"fmt"
	"os"
)

var (
	c int
	d int
	t int
	H string
	v string
	h bool
)

func init() {
	flag.BoolVar(&h, "h", false, "Help function")

	flag.StringVar(&v, "v", "", "Print version details")
	flag.StringVar(&H, "H", "Headers", "Add header to request")

	flag.IntVar(&c, "c", 5, "Connections to keep open")
	flag.IntVar(&d, "d", 3, "Duration of test")
	flag.IntVar(&t, "t", 3, "Number of threads to use")

	flag.Usage = usage
}

func usage () {
	fmt.Fprintf(os.Stderr, `
Usage: Bench <options> <url>                            
  Options:
`)

	flag.PrintDefaults()
}

func main(){
	flag.Parse()

	if h {
		flag.Usage()
	}

	tail := flag.Args()
	fmt.Printf("%t, %d, %d, %d, %s, %s", h, c, d, t, H, v)
	fmt.Printf("tail: %+q\n", tail[0])
}