package main

import (
	"fmt"
	"log"
	"os"

	"github.com/urfave/cli/v2"
)

type Bench struct {
	// command
	run_time int
	clients  int
	url      string
	method   string
}

func command() {
	app := cli.NewApp()
	var clients int
	var run_time int
	var force bool
	var method string

	app.Flags = []cli.Flag{
		&cli.IntFlag{
			Name:        "client",
			Aliases:     []string{"p"},
			Usage:       "Run <n> HTTP clients at once. Default 1.",
			Value:       1,
			Destination: &clients,
		},
		&cli.IntFlag{
			Name:        "time",
			Aliases:     []string{"t"},
			Usage:       "Run benchmark for <sec> seconds. Default 30.",
			Value:       30,
			Destination: &run_time,
		},
		&cli.BoolFlag{
			Name:        "force",
			Aliases:     []string{"f"},
			Usage:       "force function todo...",
			Value:       false,
			Destination: &force,
		},
		&cli.StringFlag{
			Name:        "method",
			Aliases:     []string{"m"},
			Usage:       "Method for http request",
			Destination: &method,
		},
	}

	app.Action = func(c *cli.Context) error {
		var output string
		methodArray := [4]string{"get", "trace", "head", "options"}
		var found bool = false
		for i := range methodArray {
			if c.String("method") == i {
				found = true
			}
			output = "Method error."
			fmt.Println(output)
			return nil
		}
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}

}

func main() {

	command()
}
