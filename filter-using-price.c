#include<stdio.h>
#include<libxml/parser.h>

void errExit(char *errMsg);
void filter(xmlDocPtr doc, xmlNodePtr cur, long price);

main(int argc, char *argv[])
{
    long price;
    char *endptr;

    xmlChar *key;
    xmlDocPtr doc;
    xmlNodePtr cur;

    if(argc != 2){
        fprintf(stderr, "usage: %s max_price\n", argv[0]);
        exit(2);
    }

    endptr = NULL;
    price = strtoul(argv[1], &endptr, 10);
    // checking price
    if(*endptr != '\0')
        errExit("invalid price\n");

    // opening the xml doc
    if((doc = xmlParseFile("library.xml")) == NULL)
        errExit("document not parsed successfully\n");

    // setting the root element
    if((cur = xmlDocGetRootElement(doc)) == NULL)
        errExit("empty document\n");

    // checking root element is "library"
    if(xmlStrcmp(cur->name, (const xmlChar *) "library")){
        xmlFreeDoc(doc);
        errExit("doc of the wrong type, root node != library\n");
    }

    cur = cur->xmlChildrenNode;

    while(cur != NULL){
        if(xmlStrcmp(cur->name, (const xmlChar *)"book") == 0) // searching for book
            filter(doc, cur, price);
        cur = cur->next;
    }

    xmlFreeDoc(doc);
    printf("\n");
}

// print books whose price is <= input price
void filter(xmlDocPtr doc, xmlNodePtr book, long price)
{
    xmlNodePtr cur;
    long bookprice;
    xmlChar *name, *author, *priceStr;

    cur = book->xmlChildrenNode;

    while(cur != NULL){
        if(xmlStrcmp(cur->name, (const xmlChar *)"name") == 0)
            name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        else if(xmlStrcmp(cur->name, (const xmlChar *)"author") == 0)
            author = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        else if(xmlStrcmp(cur->name, (const xmlChar *)"price") == 0){
            priceStr = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            bookprice = strtoul((char *)priceStr, NULL, 10);
            if(bookprice <= price)
                printf("%-5ld %s by %s\n", bookprice, name, author);
        }

        cur = cur->next;
    }

    xmlFree(name);
    xmlFree(author);
    xmlFree(priceStr);
}

void errExit(char *errMsg)
{
    fprintf(stderr, "%s", errMsg);
    exit(1);
}
