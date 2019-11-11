#include "mnist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <time.h>

struct mnist_dataset_t
{
	unsigned int width;
	unsigned int height;
	struct mnist_image_t * start;
	unsigned int imagecount;
};

struct mnist_image_t
{
	unsigned char * imagedata;
        int label;
        struct mnist_image_t * next_image;
	struct mnist_image_t * prev_image;
};

mnist_dataset_handle mnist_create(unsigned int x, unsigned int y)
{
	struct mnist_dataset_t mnist_dataset_t;
	mnist_dataset_handle h = malloc(sizeof(mnist_dataset_t));

	//if malloc fails to find space
	if (!h)
	{
		return MNIST_DATASET_INVALID;
	}

	h->width = x;	
	h->height = y;
	h->start = NULL; //no imagedata initially so pointing to null, must be freed
	h->imagecount = 0;
	
	return h;
}

void mnist_free (mnist_dataset_handle handle)
{
	if (handle->start != NULL)
	{
		mnist_image_handle imh = handle->start;
		mnist_image_handle next = NULL;
		
		for (unsigned int i=0; i<handle->imagecount; ++i)
		{
			next = imh->next_image;
			free(imh->imagedata);
			free(imh);
			imh = next;
		}
	}

	free(handle);
}

int mnist_image_count (const_mnist_dataset_handle handle)
{
	//if handle not valid, return negative imagecounts
	if (handle == MNIST_DATASET_INVALID)
	{
		return -1;
	}
	return handle->imagecount;
}

void mnist_image_size (const_mnist_dataset_handle handle, unsigned int * x, unsigned int * y)
{	
	if (handle == MNIST_DATASET_INVALID)
	{
		*x = 0;
        	*y = 0;
	}
	else
	{
		*x = handle->width;
        	*y = handle->height;
	}
}

mnist_dataset_handle mnist_open (const char * name)
{
	char dsfilename[255];
	char lsfilename[255];
	snprintf(dsfilename, sizeof(dsfilename)-1, "%s-images-idx3-ubyte", name);
	snprintf(lsfilename, sizeof(lsfilename)-1, "%s-labels-idx1-ubyte", name);
	
	//initialize standard width variables
	uint32_t magic_number;
        uint32_t num_of_items;
	uint32_t num_of_labels;
	uint32_t width;
	uint32_t height;
	
	//initialize pointers
        char * mnptr = (char*)&magic_number;
        char * itemptr = (char*)&num_of_items;
	char * numlblptr = (char*)&num_of_labels;
	char * wdthptr = (char*)&width;
	char * hgtptr = (char*)&height;

	//open files
	FILE * dataset = fopen(dsfilename, "rb");
	FILE * labelset = fopen(lsfilename, "rb");

	//if open files fails, print error, exit program
	if (!dataset || !labelset)
        {
                fprintf (stderr, "error: file open failed '%s'.\n", name);
                exit(1);
        }

	/********************************DATASET***********************************/
	//read in magic number for dataset and check if its correct
	fread(mnptr, sizeof(unsigned char), sizeof(magic_number), dataset);
        magic_number = ntohl(magic_number);

        if (magic_number != 2051)
        {
                fprintf(stderr, "error: magic number does not equal 2049.");
		exit(1);
        }

	//read in num items check correct
        fread(itemptr, sizeof(unsigned char), sizeof(num_of_items), dataset);
        num_of_items = ntohl(num_of_items);

	//read in width and height
        fread(wdthptr, sizeof(unsigned char), sizeof(width), dataset);
        fread(hgtptr, sizeof(unsigned char), sizeof(height), dataset);
        width = ntohl(width);
        height = ntohl(height);
	
	/********************************LABELS***********************************/
	//read in magic number for labelset and check if its correct
	fread(mnptr, sizeof(unsigned char), sizeof(magic_number), labelset);
        magic_number = ntohl(magic_number);

        if (magic_number != 2049)
        {
                fprintf(stderr, "error: magic number does not equal 2049.");
		exit(1);
        }

	fread(numlblptr, sizeof(unsigned char), sizeof(num_of_labels), labelset);
        num_of_labels = ntohl(num_of_labels);

	/****************************READ IN DATA + LABELS*******************************/
	//initialize new empty dataset with above height and width
	mnist_dataset_handle h =  mnist_create(width, height);	
	
	//set start parameters
	mnist_image_handle imh = mnist_image_begin(h);
	unsigned char * imagedata;
	unsigned int newlabel;
	//read in data
        for (int i=0; i < num_of_items; ++i)
        {
		imagedata = malloc((height*width*sizeof(unsigned char)));
		fread(imagedata, sizeof(unsigned char), height*width, dataset);
		imh = mnist_image_add_after(h, imh, imagedata, width, height); 
		newlabel = mnist_image_label(imh);
		newlabel = 0;
                fread(&newlabel, sizeof(unsigned char), 1, labelset);	
		mnist_set_image_label(imh, newlabel);
        }

	fclose(dataset);
	fclose(labelset);
	return h;
}

mnist_dataset_handle mnist_open_sample (const char * name, unsigned int k)
{
	mnist_dataset_handle h = mnist_open(name);
	int imagecount = mnist_image_count(h);
	unsigned int width, height;
   	mnist_image_size (h, &width, &height);

	mnist_image_handle curr = mnist_image_begin(h);

	mnist_dataset_handle newh = mnist_create(width, height);
	mnist_image_handle last = NULL;

	int res[k];
	
	srand(time(NULL));

	for (int i=0; i<k; ++i)
	{
		res[i] = rand() % imagecount;
	}
	
	for (int i=0; i<imagecount; ++i)
	{
		for (int j=0; j<k; ++j)
		{
			if (res[j] == i)
			{
				unsigned char * imagedata = malloc((height*width*sizeof(unsigned char)));
				memcpy(imagedata, curr->imagedata, (height*width*sizeof(unsigned char)));
				mnist_image_handle next = mnist_image_add_after(newh, last, imagedata, width, height);
				next->label = curr->label;
				last = next;
			}
		}

		curr = curr->next_image;
	}

	mnist_free(h);
	return newh;
	
}

mnist_image_handle mnist_image_begin (mnist_dataset_handle handle)
{
	if (!handle || handle==MNIST_DATASET_INVALID)
	{
		return MNIST_IMAGE_INVALID;
	}

	return handle->start;
}

const unsigned char * mnist_image_data (const_mnist_image_handle h)
{
	if (!h || h==MNIST_IMAGE_INVALID)
	{
                fprintf(stderr, "error: no image data");
		exit(1);
	}
	return h->imagedata;
}

int mnist_image_label (const_mnist_image_handle h)
{
	if (!h)
        {
                return -1;
        }

	return h->label;
}

int mnist_set_image_label(mnist_image_handle h, unsigned int newlabel)
{
	if (!h)
        {
                return -1;
        }

	h->label = newlabel;
	return newlabel;
}

mnist_image_handle mnist_image_next (mnist_image_handle h)
{
	if (!h)
        {
                return MNIST_IMAGE_INVALID;
        }
	
	return h->next_image;
}

mnist_image_handle mnist_image_prev (mnist_image_handle h)
{
	if (!h || h->prev_image == NULL)
	{
		return MNIST_IMAGE_INVALID;
	}
	
	return h->prev_image;
}

mnist_image_handle mnist_image_add_after (mnist_dataset_handle dataset, mnist_image_handle h, const unsigned char * imagedata, unsigned int x, unsigned int y)
{
	if(!dataset || dataset->width!=x || dataset->height!=y)
	{
		return MNIST_IMAGE_INVALID;
	}

	struct mnist_image_t mnist_image_t;
        mnist_image_handle imh = malloc(sizeof(mnist_image_t));
	imh->imagedata = (unsigned char*)imagedata;
	imh->label = -1;
	imh->next_image = NULL;

	if (!h || h == MNIST_IMAGE_INVALID)
	{
		imh->prev_image = NULL;
		dataset->start = imh;
		
	}
	else
	{
		imh->prev_image = h;
		h->next_image = imh;
	}

	++dataset->imagecount;

	return imh;
}

mnist_image_handle mnist_image_remove(mnist_dataset_handle dataset, mnist_image_handle h)
{
	if (!dataset || dataset == MNIST_DATASET_INVALID || !h || h == MNIST_IMAGE_INVALID)
	{
		return MNIST_IMAGE_INVALID;
	}

	mnist_image_handle first = mnist_image_begin(dataset);
	if (h == first)
	{
		dataset->start = first->next_image;
		first->next_image->prev_image = NULL;
		mnist_image_handle second = first->next_image;
		--dataset->imagecount;
		free(first->imagedata);
		free(first);
		return second;
	}
	else
	{
		mnist_image_handle current = first;
		for(int i =0; i<dataset->imagecount-1; ++i)
		{
			current = mnist_image_next(current);
                	if (h == current)
                	{
                        	current->prev_image->next_image = current->next_image;
				current->next_image->prev_image = current->prev_image;
				mnist_image_handle next = current->next_image;
				--dataset->imagecount;
				free(current->imagedata);
				free(current);
				return next;
                	}
       		} 
	}
	return EXIT_SUCCESS;		
}

bool mnist_save(const_mnist_dataset_handle h, const char * filename)
{
   	if (!h)
   	{
      		fprintf (stderr, "Couldn't open dataset '%s'!\n", filename);
      		return 1;
   	}

   	const unsigned int imagecount = mnist_image_count (h);
   	unsigned int width, height;
   	mnist_image_size (h, &width, &height);

	char dsfilename[255];
        char lsfilename[255];
	
	//snprintf (dsfilename, sizeof(dsfilename)-1, "%s-%05u-data.pgm", filename, i);
        //snprintf (lsfilename, sizeof(lsfilename)-1, "%s-%05u-label.pgm", filename, i);
  
	mnist_image_handle img = h->start;

	if (!img)
        {
        	fprintf (stderr, "Not enough images in dataset!\n");
                return 1;
        }

        //FILE * f = fopen (dsfilename, "w");
        //FILE * fl = fopen (lsfilename, "w");

        //if (!f || !fl)
        //{
          //      perror ("Couldn't write to file: ");
            //    return 1;
        //}

   	for (unsigned int i=0; i<imagecount; ++i)
   	{
		snprintf (dsfilename, sizeof(dsfilename)-1, "%s-%05u-data.pgm", filename, i);
        	snprintf (lsfilename, sizeof(lsfilename)-1, "%s-%05u-label.pgm", filename, i);

		FILE * f = fopen (dsfilename, "w");
        	FILE * fl = fopen (lsfilename, "w");

        	if (!f || !fl)
        	{
                	perror ("Couldn't write to file: ");
                	return 1;
        	}

      		printf ("Writing %s-%05u-data...\n", filename, i);
		printf ("Writing %s-%05u-label...\n", filename, i);

      		fputs ("P2\n", f);
      		fprintf (f, "# %s %u\n", filename, i);
      		fprintf (f, "%u %u\n", width, height);
      		fputs ("255\n", f);

      		const unsigned char * data = mnist_image_data (img);
		int lbl =  mnist_image_label(img);

      		for (unsigned int y=0; y<height; ++y)
      		{
         		for (unsigned int x=0; x<width; ++x)
         		{
            			unsigned int c = *data;
            			fprintf (f, "%u ", c);
            			++data;
         		}
         		fputs ("\n", f);
      		}

		fprintf(fl, "%d ", lbl);
		fclose (fl);
        	fclose (f);
      		img = mnist_image_next (img);
   	}
	//fclose (fl);
        //fclose (f);

   	exit(EXIT_SUCCESS);
	
}




