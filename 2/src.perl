#!/usr/bin/perl

# SQL injection homework
# Mike Rosulek
# rosulekm@eecs.oregonstate.edu
# for ECE 478: network security @ Oregon State University

## READ THESE COMMENTS IF YOU DON'T ALREADY KNOW PERL DBI:

## In Perl DBI, the method names are somewhat self-explanitory, and I also
## tried to make the variable names reasonable. But let me explain this line:
##
##     my ($first_column) = $statement->fetchrow();
##
## $statement has been executed and we are now in the "fetch results from the
## server" phase. We only expect this statement to return 1 result, so we 
## only fetchrow() once. Each row returns a list of values: we assign the
## first element of that list to $first_column, and ignore the rest of the row.


use CGI ':standard';
use DBI;

print header;

sub msg ($) { print "<p><b>Info:</b> $_[0]</p>"; }
sub err ($) { msg $_[0]; exit; }

## connect to database
my $db_handle = DBI->connect( "dbi:SQLite:dbname=sqli-hw.db", "", "",
                              {RaiseError => 0, PrintError => 0}    )
    or err $DBI::errstr;

## these are the form data sent by the web browser in the GET/POST request:
my $U = param("u"); 
my $P = param("p");

## form was submitted: verify login
if ($U) {

    my $sql = "select * from users where uname='$U' and pwd='$P'";
    my $statement = $db_handle->prepare($sql)
        or err "Can't prepare SQL statement: $DBI::errstr";
    $statement->execute()
        or err "Can't execute SQL statement: $DBI::errstr";
    my ($first_column) = $statement->fetchrow();

    if ($first_column) {
        msg "Login successful. Welcome, $first_column";
    } else {
        msg "Login failed!";
    }

## no form was submitted: show the login form
} else {

    print qq[
        Login here:
        <form>
            <input name="u">
            <input name="p" type="password">
            <input type="submit" value="go">
        </form>
    ];

}



